// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/string_util.h"
#include "base/utf_string_conversions.h"
#include "cameo/src/runtime/browser/runtime.h"
#include "cameo/src/runtime/browser/ui/color_chooser.h"
#include "cameo/src/test/base/cameo_test_utils.h"
#include "cameo/src/test/base/in_process_browser_test.h"
#include "content/public/browser/render_view_host.h"
#include "content/public/browser/web_contents.h"
#include "content/public/test/browser_test_utils.h"
#include "content/public/test/test_utils.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/shell_dialogs/select_file_dialog.h"
#include "ui/shell_dialogs/select_file_dialog_factory.h"

#if defined(TOOLKIT_GTK)
#include <gtk/gtk.h>  // NOLINT(build/include_order)
#endif

using cameo::Runtime;

namespace {

base::FilePath g_file_selected_path;

void SetSelectFileDialogReturnPath(base::FilePath path) {
  g_file_selected_path = path;
}

class TestSelectFileDialog : public ui::SelectFileDialog {
 public:
  static ui::SelectFileDialog* Create(
      Listener* listener,
      ui::SelectFilePolicy* policy) {
    return new TestSelectFileDialog(listener, policy);
  }
  virtual bool IsRunning(gfx::NativeWindow owning_window) const {
    return false;
  }
  virtual void ListenerDestroyed() {}

 protected:
  virtual void SelectFileImpl(
      Type type,
      const string16& title,
      const base::FilePath& default_path,
      const FileTypeInfo* file_types,
      int file_type_index,
      const base::FilePath::StringType& default_extension,
      gfx::NativeWindow owning_window,
      void* params) {
    listener_->FileSelected(g_file_selected_path, 1, NULL);
  }

 private:
  virtual bool HasMultipleFileTypeChoicesImpl() {
    return false;
  }
  TestSelectFileDialog(Listener* listener, ui::SelectFilePolicy* policy) :
        ui::SelectFileDialog(listener, policy),
        listener_(listener) {
  }
  Listener* listener_;
};

class TestSelectFileDialogFactory : public ui::SelectFileDialogFactory {
 public:
  virtual ui::SelectFileDialog* Create(
      ui::SelectFileDialog::Listener* listener,
      ui::SelectFilePolicy* policy) {
    return TestSelectFileDialog::Create(listener, policy);
  }
};

}  // namespace

class CameoFormInputTest : public InProcessBrowserTest {
 public:
  CameoFormInputTest() {
    ui::SelectFileDialog::SetFactory(&factory_);
  }

  virtual ~CameoFormInputTest() {
  }

  void SetBrowserTestColor(unsigned int r, unsigned int g, unsigned int b) {
    cameo::ColorChooser::SetColorForBrowserTest(SkColorSetRGB(r, g, b));
  }

 private:
  TestSelectFileDialogFactory factory_;
};

IN_PROC_BROWSER_TEST_F(CameoFormInputTest, FileSelector) {
  SetSelectFileDialogReturnPath(cameo_test_utils::GetTestFilePath(
      base::FilePath(), base::FilePath().AppendASCII("file_to_select")));
  GURL url = cameo_test_utils::GetTestURL(
      base::FilePath(), base::FilePath().AppendASCII("form_input.html"));
  cameo_test_utils::NavigateToURL(runtime(), url);
  content::WaitForLoadStop(runtime()->web_contents());
  runtime()->web_contents()->GetRenderViewHost()->ExecuteJavascriptInWebFrame(
      string16(),
      ASCIIToUTF16("doSelectFile();"));
  content::RunAllPendingInMessageLoop();
  string16 expected_title = ASCIIToUTF16("file selected: ");
  expected_title.append(
      ASCIIToUTF16(g_file_selected_path.BaseName().MaybeAsASCII()));
  content::TitleWatcher title_watcher(runtime()->web_contents(),
                                      expected_title);
  EXPECT_EQ(title_watcher.WaitAndGetTitle(), expected_title);
}

IN_PROC_BROWSER_TEST_F(CameoFormInputTest, ColorChooser) {
  unsigned int r = 23, g = 174, b = 98;
  SetBrowserTestColor(r, g, b);
  GURL url = cameo_test_utils::GetTestURL(
      base::FilePath(), base::FilePath().AppendASCII("form_input.html"));
  cameo_test_utils::NavigateToURL(runtime(), url);
  content::WaitForLoadStop(runtime()->web_contents());
  runtime()->web_contents()->GetRenderViewHost()->ExecuteJavascriptInWebFrame(
      string16(),
      ASCIIToUTF16("doChooseColor();"));
  content::RunAllPendingInMessageLoop();
  string16 expected_title = ASCIIToUTF16("color chosen: ");
  char rgb[8];
  base::snprintf(rgb, sizeof(rgb), "#%02x%02x%02x", r, g, b);
  expected_title.append(ASCIIToUTF16(rgb));
  content::TitleWatcher title_watcher(runtime()->web_contents(),
                                      expected_title);
  EXPECT_EQ(title_watcher.WaitAndGetTitle(), expected_title);
}
