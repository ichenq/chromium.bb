// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DOMWindow_h
#define DOMWindow_h

#include "core/events/EventTarget.h"
#include "core/frame/DOMWindowBase64.h"
#include "platform/heap/Handle.h"
#include "platform/scroll/ScrollableArea.h"

#include "wtf/Forward.h"

namespace blink {

class ApplicationCache;
class BarProp;
class CSSRuleList;
class CSSStyleDeclaration;
class Console;
class DOMSelection;
class DOMWindowCSS;
class Document;
class Element;
class Frame;
class History;
class LocalDOMWindow;
class Location;
class MediaQueryList;
class Navigator;
class Performance;
class RequestAnimationFrameCallback;
class Screen;
class ScrollToOptions;
class SerializedScriptValue;
class Storage;
class StyleMedia;

typedef WillBeHeapVector<RefPtrWillBeMember<MessagePort>, 1> MessagePortArray;

class DOMWindow : public EventTargetWithInlineData, public RefCountedWillBeNoBase<DOMWindow>, public DOMWindowBase64 {
    DEFINE_WRAPPERTYPEINFO();
    REFCOUNTED_EVENT_TARGET(DOMWindow);
public:
    // RefCountedWillBeGarbageCollectedFinalized overrides:
    void trace(Visitor* visitor) override
    {
        EventTargetWithInlineData::trace(visitor);
    }

    virtual bool isLocalDOMWindow() const { return false; }
    virtual bool isRemoteDOMWindow() const { return false; }

    virtual Frame* frame() const = 0;

    // DOM Level 0
    virtual Screen* screen() const = 0;
    virtual History* history() const = 0;
    virtual BarProp* locationbar() const = 0;
    virtual BarProp* menubar() const = 0;
    virtual BarProp* personalbar() const = 0;
    virtual BarProp* scrollbars() const = 0;
    virtual BarProp* statusbar() const = 0;
    virtual BarProp* toolbar() const = 0;
    virtual Navigator* navigator() const = 0;
    Navigator* clientInformation() const { return navigator(); }
    // FIXME: Temporary, until window.location is implemented for remote frames.
    virtual Location* location() const = 0;

    virtual bool offscreenBuffering() const = 0;

    virtual int outerHeight() const = 0;
    virtual int outerWidth() const = 0;
    virtual int innerHeight() const = 0;
    virtual int innerWidth() const = 0;
    virtual int screenX() const = 0;
    virtual int screenY() const = 0;
    int screenLeft() const { return screenX(); }
    int screenTop() const { return screenY(); }
    virtual double scrollX() const = 0;
    virtual double scrollY() const = 0;
    double pageXOffset() const { return scrollX(); }
    double pageYOffset() const { return scrollY(); }

    bool closed() const;

    // FIXME: This is not listed as a cross-origin accessible attribute, but in
    // Blink, it's currently marked as DoNotCheckSecurity.
    unsigned length() const;

    virtual const AtomicString& name() const = 0;
    virtual void setName(const AtomicString&) = 0;

    virtual String status() const = 0;
    virtual void setStatus(const String&) = 0;
    virtual String defaultStatus() const = 0;
    virtual void setDefaultStatus(const String&) = 0;

    // Self-referential attributes
    DOMWindow* self() const;
    DOMWindow* window() const { return self(); }
    DOMWindow* frames() const { return self(); }

    DOMWindow* opener() const;
    DOMWindow* parent() const;
    DOMWindow* top() const;

    // DOM Level 2 AbstractView Interface
    virtual Document* document() const = 0;

    // CSSOM View Module
    virtual StyleMedia* styleMedia() const = 0;

    // WebKit extensions
    virtual double devicePixelRatio() const = 0;

    // HTML 5 key/value storage
    virtual Storage* sessionStorage(ExceptionState&) const = 0;
    virtual Storage* localStorage(ExceptionState&) const = 0;
    virtual ApplicationCache* applicationCache() const = 0;

    // This is the interface orientation in degrees. Some examples are:
    //  0 is straight up; -90 is when the device is rotated 90 clockwise;
    //  90 is when rotated counter clockwise.
    virtual int orientation() const = 0;

    virtual Console* console() const  = 0;

    virtual Performance* performance() const = 0;

    virtual DOMWindowCSS* css() const = 0;

    virtual DOMSelection* getSelection() = 0;

    virtual void focus(ExecutionContext*) = 0;
    virtual void blur() = 0;
    virtual void close(ExecutionContext*) = 0;
    virtual void print() = 0;
    virtual void stop() = 0;

    virtual void alert(const String& message = String()) = 0;
    virtual bool confirm(const String& message) = 0;
    virtual String prompt(const String& message, const String& defaultValue) = 0;

    virtual bool find(const String&, bool caseSensitive, bool backwards, bool wrap, bool wholeWord, bool searchInFrames, bool showDialog) const = 0;

    virtual void scrollBy(double x, double y, ScrollBehavior = ScrollBehaviorAuto) const = 0;
    virtual void scrollBy(const ScrollToOptions&) const = 0;
    virtual void scrollTo(double x, double y) const = 0;
    virtual void scrollTo(const ScrollToOptions&) const = 0;
    void scroll(double x, double y) const { scrollTo(x, y); }
    void scroll(const ScrollToOptions& scrollToOptions) const { scrollTo(scrollToOptions); }
    virtual void moveBy(float x, float y) const = 0;
    virtual void moveTo(float x, float y) const = 0;

    virtual void resizeBy(float x, float y) const = 0;
    virtual void resizeTo(float width, float height) const = 0;

    virtual PassRefPtrWillBeRawPtr<MediaQueryList> matchMedia(const String&) = 0;

    // DOM Level 2 Style Interface
    virtual PassRefPtrWillBeRawPtr<CSSStyleDeclaration> getComputedStyle(Element*, const String& pseudoElt) const = 0;

    // WebKit extensions
    virtual PassRefPtrWillBeRawPtr<CSSRuleList> getMatchedCSSRules(Element*, const String& pseudoElt) const = 0;

    // WebKit animation extensions
    virtual int requestAnimationFrame(RequestAnimationFrameCallback*) = 0;
    virtual int webkitRequestAnimationFrame(RequestAnimationFrameCallback*) = 0;
    virtual void cancelAnimationFrame(int id) = 0;

    void captureEvents() { }
    void releaseEvents() { }

    // FIXME: This handles both window[index] and window.frames[index]. However,
    // the spec exposes window.frames[index] across origins but not
    // window[index]...
    DOMWindow* anonymousIndexedGetter(uint32_t) const;

    virtual void postMessage(PassRefPtr<SerializedScriptValue> message, const MessagePortArray*, const String& targetOrigin, LocalDOMWindow* source, ExceptionState&) = 0;

    // FIXME: These should be non-virtual, but this is blocked on the security
    // origin replication work.
    virtual String sanitizedCrossDomainAccessErrorMessage(LocalDOMWindow* callingWindow) = 0;
    virtual String crossDomainAccessErrorMessage(LocalDOMWindow* callingWindow) = 0;

    DEFINE_ATTRIBUTE_EVENT_LISTENER(animationend);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(animationiteration);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(animationstart);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(search);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(transitionend);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(wheel);

    DEFINE_MAPPED_ATTRIBUTE_EVENT_LISTENER(webkitanimationstart, webkitAnimationStart);
    DEFINE_MAPPED_ATTRIBUTE_EVENT_LISTENER(webkitanimationiteration, webkitAnimationIteration);
    DEFINE_MAPPED_ATTRIBUTE_EVENT_LISTENER(webkitanimationend, webkitAnimationEnd);
    DEFINE_MAPPED_ATTRIBUTE_EVENT_LISTENER(webkittransitionend, webkitTransitionEnd);

    DEFINE_ATTRIBUTE_EVENT_LISTENER(orientationchange);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(touchstart);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(touchmove);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(touchend);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(touchcancel);
};

} // namespace blink

#endif // DOMWindow_h
