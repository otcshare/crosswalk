// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Implementation of the W3C's Raw Socket API.
// http://www.w3.org/2012/sysapps/raw-sockets/

// The ReadyStateObserver is a proxy object that will
// subscribe to the parent's |readystate| event. An object
// cannot subscribe to it's own events otherwise it will
// leak (because it creates a circular reference).
//
var ReadyStateObserver = function(object_id) {
  BindingObject.call(this, object_id);
  EventTarget.call(this);

  this._addEvent("readystate");
  this.readyState = "connecting";

  var that = this;
  this.onreadystate = function(event) {
    that.readyState = event.data;
  };

  this.destructor = function() {
    this.onreadystate = null;
  };
};

ReadyStateObserver.prototype = new EventTargetPrototype();

// TCPSocket interface.
//
// TODO(tmpsantos): We are currently not throwing any exceptions
// neither validating the input parameters.
//
// TODO(tmpsantos): TCPOptions argument is being ignored by now.
//
var TCPSocket = function(remoteAddress, remotePort, options) {
  BindingObject.call(this);
  EventTarget.call(this);

  internal.postMessage("TCPSocketConstructor", [this._id]);

  this._addMethod("_close");
  this._addMethod("halfclose");
  this._addMethod("suspend");
  this._addMethod("resume");
  this._addMethod("_sendString");

  this._addEvent("drain");
  this._addEvent("open");
  this._addEvent("close");
  this._addEvent("halfclose");
  this._addEvent("error");
  this._addEvent("data");

  function sendWrapper(data) {
    this._sendString(data);

    // FIXME(tmpsantos): The spec says that send() should always
    // return which makes it *sigh* synchronous.
    return true;
  };

  function closeWrapper(data) {
    if (this._readyStateObserver.readyState = "closed")
      return;

    this._readyStateObserver.readyState = "closing";
    this._close();
  };

  Object.defineProperties(this, {
    "_readyStateObserver": {
      value: new ReadyStateObserver(this._id),
    },
    "_readyStateObserverDeleter": {
      value: v8tools.lifecycleTracker(),
    },
    "send": {
      value: sendWrapper,
      enumerable: true,
    },
    "close": {
      value: closeWrapper,
      enumerable: true,
    },
    "remoteAddress": {
      value: remoteAddress,
      enumerable: true,
    },
    "remotePort": {
      value: remotePort,
      enumerable: true,
    },
    "localAddress": {
      value: "127.0.0.1",
      enumerable: true,
    },
    "localPort": {
      value: 0,
      enumerable: true,
    },
    "bufferAmount": {
      value: 0,
      enumerable: true,
    },
    "readyState": {
      get: function() { return this._readyStateObserver.readyState; },
      enumerable: true,
    },
  });

  var watcher = this._readyStateObserver;
  this._readyStateObserverDeleter.destructor = function() {
    watcher.destructor();
  };

  // This is needed, otherwise events like "error" can get fired before
  // we give the user a chance to register a listener.
  function delayedInitialization(obj) {
    obj._postMessage("init", [remoteAddress, remotePort, options]);
  };

  setTimeout(delayedInitialization, 0, this);
};

TCPSocket.prototype = new EventTargetPrototype();
TCPSocket.prototype.constructor = TCPSocket;

exports.TCPSocket = TCPSocket;
