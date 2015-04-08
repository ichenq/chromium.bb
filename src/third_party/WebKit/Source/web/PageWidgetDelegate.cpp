/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "web/PageWidgetDelegate.h"

#include "core/frame/FrameView.h"
#include "core/frame/LocalFrame.h"
#include "core/page/AutoscrollController.h"
#include "core/page/EventHandler.h"
#include "core/page/Page.h"
#include "core/rendering/RenderView.h"
#include "core/rendering/compositing/RenderLayerCompositor.h"
#include "platform/Logging.h"
#include "platform/graphics/GraphicsContext.h"
#include "public/web/WebInputEvent.h"
#include "web/PageOverlayList.h"
#include "web/WebInputEventConversion.h"
#include "wtf/CurrentTime.h"

namespace blink {

void PageWidgetDelegate::animate(Page& page, double monotonicFrameBeginTime, LocalFrame& root)
{
    RefPtrWillBeRawPtr<FrameView> view = root.view();
    if (!view)
        return;
    page.autoscrollController().animate(monotonicFrameBeginTime);
    page.animator().serviceScriptedAnimations(monotonicFrameBeginTime);
}

void PageWidgetDelegate::layout(Page& page, LocalFrame& root)
{
    page.animator().updateLayoutAndStyleForPainting(&root);
}

void PageWidgetDelegate::paint(Page& page, PageOverlayList* overlays, WebCanvas* canvas, const WebRect& rect, CanvasBackground background, LocalFrame& root)
{
    if (rect.isEmpty())
        return;
    GraphicsContext gc(canvas, nullptr);
    gc.setCertainlyOpaque(background == Opaque);
    float scaleFactor = page.deviceScaleFactor();
    gc.scale(scaleFactor, scaleFactor);
    gc.setDeviceScaleFactor(scaleFactor);
    IntRect dirtyRect(rect);
    gc.save(); // Needed to save the canvas, not the GraphicsContext.
    FrameView* view = root.view();
    if (view) {
        gc.clip(dirtyRect);
        view->paint(&gc, dirtyRect);
        if (overlays)
            overlays->paintWebFrame(gc);
    } else {
        gc.fillRect(dirtyRect, Color::white);
    }
    gc.restore();
}

bool PageWidgetDelegate::handleInputEvent(PageWidgetEventHandler& handler, const WebInputEvent& event, LocalFrame* root)
{
    switch (event.type) {

    // FIXME: WebKit seems to always return false on mouse events processing
    // methods. For now we'll assume it has processed them (as we are only
    // interested in whether keyboard events are processed).
    // FIXME: Why do we return true when there is no root or the root is
    // detached?
    case WebInputEvent::MouseMove:
        if (!root || !root->view())
            return true;
        handler.handleMouseMove(*root, static_cast<const WebMouseEvent&>(event));
        return true;
    case WebInputEvent::MouseLeave:
        if (!root || !root->view())
            return true;
        handler.handleMouseLeave(*root, static_cast<const WebMouseEvent&>(event));
        return true;
    case WebInputEvent::MouseDown:
        if (!root || !root->view())
            return true;
        handler.handleMouseDown(*root, static_cast<const WebMouseEvent&>(event));
        return true;
    case WebInputEvent::MouseUp:
        if (!root || !root->view())
            return true;
        handler.handleMouseUp(*root, static_cast<const WebMouseEvent&>(event));
        return true;

    case WebInputEvent::MouseWheel:
        if (!root || !root->view())
            return false;
        return handler.handleMouseWheel(*root, static_cast<const WebMouseWheelEvent&>(event));

    case WebInputEvent::RawKeyDown:
    case WebInputEvent::KeyDown:
    case WebInputEvent::KeyUp:
        return handler.handleKeyEvent(static_cast<const WebKeyboardEvent&>(event));

    case WebInputEvent::Char:
        return handler.handleCharEvent(static_cast<const WebKeyboardEvent&>(event));
    case WebInputEvent::GestureScrollBegin:
    case WebInputEvent::GestureScrollEnd:
    case WebInputEvent::GestureScrollUpdate:
    case WebInputEvent::GestureFlingStart:
    case WebInputEvent::GestureFlingCancel:
    case WebInputEvent::GestureTap:
    case WebInputEvent::GestureTapUnconfirmed:
    case WebInputEvent::GestureTapDown:
    case WebInputEvent::GestureShowPress:
    case WebInputEvent::GestureTapCancel:
    case WebInputEvent::GestureDoubleTap:
    case WebInputEvent::GestureTwoFingerTap:
    case WebInputEvent::GestureLongPress:
    case WebInputEvent::GestureLongTap:
        return handler.handleGestureEvent(static_cast<const WebGestureEvent&>(event));

    case WebInputEvent::TouchStart:
    case WebInputEvent::TouchMove:
    case WebInputEvent::TouchEnd:
    case WebInputEvent::TouchCancel:
        if (!root || !root->view())
            return false;
        return handler.handleTouchEvent(*root, static_cast<const WebTouchEvent&>(event));

    case WebInputEvent::GesturePinchBegin:
    case WebInputEvent::GesturePinchEnd:
    case WebInputEvent::GesturePinchUpdate:
        // FIXME: Once PlatformGestureEvent is updated to support pinch, this
        // should call handleGestureEvent, just like it currently does for
        // gesture scroll.
        return false;

    default:
        return false;
    }
}

// ----------------------------------------------------------------
// Default handlers for PageWidgetEventHandler

void PageWidgetEventHandler::handleMouseMove(LocalFrame& mainFrame, const WebMouseEvent& event)
{
    mainFrame.eventHandler().handleMouseMoveEvent(PlatformMouseEventBuilder(mainFrame.view(), event));
}

void PageWidgetEventHandler::handleMouseLeave(LocalFrame& mainFrame, const WebMouseEvent& event)
{
    mainFrame.eventHandler().handleMouseLeaveEvent(PlatformMouseEventBuilder(mainFrame.view(), event));
}

void PageWidgetEventHandler::handleMouseDown(LocalFrame& mainFrame, const WebMouseEvent& event)
{
    mainFrame.eventHandler().handleMousePressEvent(PlatformMouseEventBuilder(mainFrame.view(), event));
}

void PageWidgetEventHandler::handleMouseUp(LocalFrame& mainFrame, const WebMouseEvent& event)
{
    mainFrame.eventHandler().handleMouseReleaseEvent(PlatformMouseEventBuilder(mainFrame.view(), event));
}

bool PageWidgetEventHandler::handleMouseWheel(LocalFrame& mainFrame, const WebMouseWheelEvent& event)
{
    return mainFrame.eventHandler().handleWheelEvent(PlatformWheelEventBuilder(mainFrame.view(), event));
}

bool PageWidgetEventHandler::handleTouchEvent(LocalFrame& mainFrame, const WebTouchEvent& event)
{
    return mainFrame.eventHandler().handleTouchEvent(PlatformTouchEventBuilder(mainFrame.view(), event));
}

} // namespace blink
