/*
 * Copyright (C) 2014 Bloomberg Finance L.P.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS," WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <blpwtk2_profilecreateparams.h>

#include <base/logging.h>  // for DCHECK

namespace blpwtk2 {

ProfileCreateParams::ProfileCreateParams(const StringRef& dataDir)
: d_dataDir(dataDir)
, d_diskCacheEnabled(!dataDir.isEmpty())
, d_cookiePersistenceEnabled(!dataDir.isEmpty())
{
}

void ProfileCreateParams::setDiskCacheEnabled(bool enabled)
{
    DCHECK(!enabled || !d_dataDir.isEmpty());
    d_diskCacheEnabled = enabled;
}

void ProfileCreateParams::setCookiePersistenceEnabled(bool enabled)
{
    DCHECK(!enabled || !d_dataDir.isEmpty());
    d_cookiePersistenceEnabled = enabled;
}

StringRef ProfileCreateParams::dataDir() const
{
    return d_dataDir;
}

bool ProfileCreateParams::diskCacheEnabled() const
{
    return d_diskCacheEnabled;
}

bool ProfileCreateParams::cookiePersistenceEnabled() const
{
    return d_cookiePersistenceEnabled;
}

}  // close namespace blpwtk2

