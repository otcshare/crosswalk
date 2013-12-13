// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_APPLICATION_BROWSER_APPLICATION_SERVICE_H_
#define XWALK_APPLICATION_BROWSER_APPLICATION_SERVICE_H_

#include <string>
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/observer_list.h"
#include "xwalk/application/browser/application.h"
#include "xwalk/application/browser/application_store.h"
#include "xwalk/runtime/browser/runtime_context.h"
#include "xwalk/application/common/application_data.h"

namespace xwalk {
class RuntimeContext;
}

namespace xwalk {
namespace application {

// This will manage application install, uninstall, update and so on.
class ApplicationService : public Application::Observer {
 public:
  // Client code may use this class (and register with AddObserver below) to
  // keep track of applications installed/uninstalled.
  class Observer {
   public:
    virtual void OnApplicationInstalled(const std::string& app_id) {}
    virtual void OnApplicationUninstalled(const std::string& app_id) {}
   protected:
    ~Observer() {}
  };

  explicit ApplicationService(xwalk::RuntimeContext* runtime_context);
  virtual ~ApplicationService();

  bool Install(const base::FilePath& path, std::string* id);
  bool Uninstall(const std::string& id);
  Application* Launch(const std::string& id);
  Application* Launch(const base::FilePath& path);

  const ScopedVector<Application>& active_applications() const {
      return applications_; }

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  ApplicationStore* application_store();

 private:
  // Implementation of Application::Observer
  virtual void OnApplicationTerminated(Application*) OVERRIDE;

  Application* Launch(scoped_refptr<const ApplicationData> application_data);

  xwalk::RuntimeContext* runtime_context_;
  scoped_ptr<ApplicationStore> app_store_;
  ScopedVector<Application> applications_;
  ObserverList<Observer> observers_;

  DISALLOW_COPY_AND_ASSIGN(ApplicationService);
};

}  // namespace application
}  // namespace xwalk

#endif  // XWALK_APPLICATION_BROWSER_APPLICATION_SERVICE_H_
