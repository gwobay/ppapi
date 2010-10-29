// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPP_INSTANCE_H_
#define PPAPI_C_PPP_INSTANCE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_resource.h"

struct PP_InputEvent;
struct PP_Var;

#define PPP_INSTANCE_INTERFACE "PPP_Instance;0.2"

/**
 * @file
 * Defines the API ...
 *
 * @addtogroup PPP
 * @{
 */

struct PPP_Instance {
  /**
   * Called when a new plugin is instantiated on the web page. The identifier
   * of the new instance will be passed in as the first argument (this value is
   * generated by the browser and is an opaque handle).
   *
   * It's possible for more than one plugin instance to be created within the
   * same module (i.e. you may get more than one OnCreate without an OnDestroy
   * in between).
   *
   * If the plugin reports failure from this function, the plugin will be
   * deleted and OnDestroy will be called.
   */
  PP_Bool (*DidCreate)(PP_Instance instance,
                       uint32_t argc,
                       const char* argn[],
                       const char* argv[]);

  /**
   * Called when the plugin instance is destroyed. This will always be called,
   * even if Create returned failure. The plugin should deallocate any data
   * associated with the instance.
   */
  void (*DidDestroy)(PP_Instance instance);

  /**
   * Called when the position, the size, or the clip rect has changed.
   *
   * The |position| is the location on the page of this plugin instance. This is
   * relative to the top left corner of the viewport, which changes as the page
   * is scrolled.
   *
   * The |clip| indicates the visible region of the plugin instance. This is
   * relative to the top left of the plugin's coordinate system (not the page).
   * If the plugin is invisible, the clip rect will be (0, 0, 0, 0).
   */
  void (*DidChangeView)(PP_Instance instance,
                        const struct PP_Rect* position,
                        const struct PP_Rect* clip);

  /**
   * Notification that the given plugin instance has gained or lost focus.
   * Having focus means that keyboard events will be sent to your plugin
   * instance. A plugin's default condition is that it will not have focus.
   *
   * Note: clicks on your plugins will give focus only if you handle the
   * click event. You signal if you handled it by returning PP_TRUE from
   * HandleInputEvent. Otherwise the browser will bubble the event and give
   * focus to the element on the page that actually did end up consuming it.
   * If you're not getting focus, check to make sure you're returning PP_TRUE
   * from the mouse click in HandleInputEvent.
   */
  void (*DidChangeFocus)(PP_Instance instance, PP_Bool has_focus);

  /**
   * General handler for input events. Returns PP_TRUE if the event was handled
   * or PP_FALSE if it was not.
   *
   * If the event was handled, it will not be forwarded to the web page or
   * browser. If it was not handled, it will bubble according to the normal
   * rules. So it is important that a plugin respond accurately with whether
   * event propogation should continue.
   *
   * Event propogation also controls focus. If you handle an event like a mouse
   * event, typically your plugin will be given focus. Returning PP_FALSE means
   * that the click will be given to a lower part of the page and the plugin
   * will not receive focus. This allows a plugin to be partially transparent,
   * where clicks on the transparent areas will behave like clicks to the
   * underlying page.
   */
  PP_Bool (*HandleInputEvent)(PP_Instance instance,
                              const struct PP_InputEvent* event);

  /**
   * Called after Initialize for a full-frame plugin that was instantiated
   * based on the MIME type of a DOMWindow navigation. This only applies to
   * plugins that are registered to handle certain MIME types (not current
   * Native Client plugins).
   *
   * The given url_loader corresponds to a PPB_URLLoader instance that is
   * already opened. Its response headers may be queried using
   * PPB_URLLoader::GetResponseInfo. The url loader is not addrefed on behalf
   * of the plugin, if you're going to keep a reference to it, you need to
   * addref it yourself.
   *
   * This method returns PP_FALSE if the plugin cannot handle the data. In
   * response to this method, the plugin should call ReadResponseBody to read
   * the incoming data.
   */
  PP_Bool (*HandleDocumentLoad)(PP_Instance instance, PP_Resource url_loader);

  /**
   * Returns a Var representing the instance object to the web page. Normally
   * this will be a PPP_Class object that exposes certain methods the page
   * may want to call.
   *
   * On Failure, the returned var should be a "void" var.
   *
   * The returned PP_Var should have a reference added for the caller, which
   * will be responsible for Release()ing that reference.
   */
  struct PP_Var (*GetInstanceObject)(PP_Instance instance);
};

/**
 * @}
 * End addtogroup PPP
 */
#endif  // PPAPI_C_PPP_INSTANCE_H_
