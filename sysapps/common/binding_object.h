// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_SYSAPPS_COMMON_BINDING_OBJECT_H_
#define XWALK_SYSAPPS_COMMON_BINDING_OBJECT_H_

#include <string>
#include "xwalk/extensions/browser/xwalk_extension_function_handler.h"
#include "xwalk/extensions/browser/xwalk_extension_internal.h"

namespace xwalk {
namespace sysapps {

using extensions::XWalkExtensionFunctionHandler;
using extensions::XWalkInternalExtensionInstance;

// This class is used to represent every object living in the JavaScript
// context that has a counterpart in the native side. The lifecycle of
// this object is controlled by the BindingObjectStore.
class BindingObject : public XWalkExtensionFunctionHandler {
 public:
  BindingObject();
  virtual ~BindingObject();

  void set_instance(XWalkInternalExtensionInstance* instance) {
    instance_ = instance;
  }

 protected:
  void PostResult(const std::string& callback_id,
                  scoped_ptr<base::ListValue> result);

 private:
  XWalkInternalExtensionInstance* instance_;
};

}  // namespace sysapps
}  // namespace xwalk

#endif  // XWALK_SYSAPPS_COMMON_BINDING_OBJECT_H_
