// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_APPLICATION_BROWSER_APPLICATION_H_
#define XWALK_APPLICATION_BROWSER_APPLICATION_H_

#include <map>
#include <set>
#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "xwalk/application/browser/event_observer.h"
#include "xwalk/application/common/application_data.h"
#include "xwalk/runtime/browser/runtime.h"


namespace xwalk {

class RuntimeContext;

namespace application {

class ApplicationHost;
class Manifest;

class Application : public Runtime::Observer {
 public:
  class Observer {
   public:
    virtual void OnApplicationTerminated(Application*) {}

   protected:
    ~Observer() {}
  };

  Application(scoped_refptr<ApplicationData> data,
              RuntimeContext* context, Observer* observer);
  virtual ~Application();

  bool Launch();
  bool is_active() const { return !runtimes_.empty(); }
  void Close();

  bool HasMainDocument() const { return application_data_->HasMainDocument(); }
  std::string id() const { return application_data_->ID(); }

  Runtime* GetMainDocumentRuntime() const;

  int GetRenderProcessHostID() const;

  const ApplicationData* data() const { return application_data_; }
  ApplicationData* data() { return application_data_; }

 protected:
  // Runtime::Observer implementation.
  virtual void OnRuntimeAdded(Runtime* runtime) OVERRIDE;
  virtual void OnRuntimeRemoved(Runtime* runtime) OVERRIDE;

 private:
  friend class FinishEventObserver;
  bool RunMainDocument();
  bool RunFromLocalPath();
  void CloseMainDocument();
  bool IsOnSuspendHandlerRegistered(const std::string& app_id) const;

  RuntimeContext* runtime_context_;
  scoped_refptr<ApplicationData> application_data_;
  Runtime* main_runtime_;
  std::set<Runtime*> runtimes_;
  scoped_ptr<EventObserver> finish_observer_;
  Observer* observer_;

  DISALLOW_COPY_AND_ASSIGN(Application);
};

}  // namespace application
}  // namespace xwalk

#endif  // XWALK_APPLICATION_BROWSER_APPLICATION_H_
