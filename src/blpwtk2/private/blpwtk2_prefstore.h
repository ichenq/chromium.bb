/*
 * Copyright (C) 2013 Bloomberg Finance L.P.
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

#ifndef INCLUDED_BLPWTK2_PREFSTORE_H
#define INCLUDED_BLPWTK2_PREFSTORE_H

#include <blpwtk2_config.h>

#include <base/prefs/persistent_pref_store.h>
#include <base/prefs/pref_value_map.h>
#include <base/observer_list.h>

namespace blpwtk2 {

class PrefStore : public PersistentPrefStore {
  public:
    PrefStore();
    virtual ~PrefStore();

    // PrefStore
    virtual void AddObserver(PrefStore::Observer* observer) OVERRIDE;
    virtual void RemoveObserver(PrefStore::Observer* observer) OVERRIDE;
    virtual bool HasObservers() const OVERRIDE;
    virtual bool IsInitializationComplete() const OVERRIDE;
    virtual bool GetValue(const std::string& key,
                        const base::Value** result) const OVERRIDE;

    // PersistentPrefStore
    virtual bool GetMutableValue(const std::string& key,
                                base::Value** result) OVERRIDE;
    virtual void SetValue(const std::string& key, base::Value* value) OVERRIDE;
    virtual void SetValueSilently(const std::string& key,
                                base::Value* value) OVERRIDE;
    virtual void RemoveValue(const std::string& key) OVERRIDE;
    virtual void MarkNeedsEmptyValue(const std::string& key) OVERRIDE;
    virtual bool ReadOnly() const OVERRIDE;
    virtual PrefReadError GetReadError() const OVERRIDE;
    virtual PrefReadError ReadPrefs() OVERRIDE;
    virtual void ReadPrefsAsync(ReadErrorDelegate* delegate) OVERRIDE;
    virtual void CommitPendingWrite() OVERRIDE;
    virtual void ReportValueChanged(const std::string& key) OVERRIDE;

private:
    void OnInitializationCompleted();

    ObserverList<PrefStore::Observer, true> d_observers;
    PrefValueMap d_prefs;
};

}  // close namespace blpwtk2

#endif  // INCLUDED_BLPWTK2_PREFSTORE_H
