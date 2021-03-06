// Copyright (c) 2015 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/ui/win/electron_desktop_window_tree_host_win.h"

#include "ui/base/win/hwnd_metrics.h"

namespace electron {

ElectronDesktopWindowTreeHostWin::ElectronDesktopWindowTreeHostWin(
    NativeWindowViews* native_window_view,
    views::DesktopNativeWidgetAura* desktop_native_widget_aura)
    : views::DesktopWindowTreeHostWin(native_window_view->widget(),
                                      desktop_native_widget_aura),
      native_window_view_(native_window_view) {}

ElectronDesktopWindowTreeHostWin::~ElectronDesktopWindowTreeHostWin() {}

bool ElectronDesktopWindowTreeHostWin::PreHandleMSG(UINT message,
                                                    WPARAM w_param,
                                                    LPARAM l_param,
                                                    LRESULT* result) {
  return native_window_view_->PreHandleMSG(message, w_param, l_param, result);
}

bool ElectronDesktopWindowTreeHostWin::ShouldPaintAsActive() const {
  // Tell Chromium to use system default behavior when rendering inactive
  // titlebar, otherwise it would render inactive titlebar as active under
  // some cases.
  // See also https://github.com/electron/electron/issues/24647.
  return false;
}

bool ElectronDesktopWindowTreeHostWin::HasNativeFrame() const {
  // Since we never use chromium's titlebar implementation, we can just say
  // that we use a native titlebar. This will disable the repaint locking when
  // DWM composition is disabled.
  return true;
}

bool ElectronDesktopWindowTreeHostWin::GetClientAreaInsets(
    gfx::Insets* insets,
    HMONITOR monitor) const {
  if (IsMaximized() && !native_window_view_->has_frame()) {
    // Windows automatically adds a standard width border to all sides when a
    // window is maximized.
    int frame_thickness = ui::GetFrameThickness(monitor) - 1;
    *insets = gfx::Insets(frame_thickness, frame_thickness, frame_thickness,
                          frame_thickness);
    return true;
  }
  return false;
}

}  // namespace electron
