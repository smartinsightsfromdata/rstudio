/*
 * AceEditorNative.java
 *
 * Copyright (C) 2009-12 by RStudio, Inc.
 *
 * Unless you have received this program directly from RStudio pursuant
 * to the terms of a commercial license agreement with RStudio, then
 * this program is licensed to you under the terms of version 3 of the
 * GNU Affero General Public License. This program is distributed WITHOUT
 * ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THOSE OF NON-INFRINGEMENT,
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Please refer to the
 * AGPL (http://www.gnu.org/licenses/agpl-3.0.txt) for more details.
 *
 */
package org.rstudio.studio.client.workbench.views.source.editors.text.ace;

import com.google.gwt.core.client.JavaScriptObject;
import com.google.gwt.dom.client.Element;
import com.google.gwt.event.shared.HandlerRegistration;
import com.google.gwt.event.shared.HasHandlers;
import com.google.gwt.user.client.Command;

import org.rstudio.core.client.CommandWithArg;
import java.util.LinkedList;

public class AceEditorNative extends JavaScriptObject {

   protected AceEditorNative() {}

   public native final EditSession getSession() /*-{
      return this.getSession();
   }-*/;

   public native final Renderer getRenderer() /*-{
      return this.renderer;
   }-*/;

   public native final void resize() /*-{
      this.resize();
   }-*/;

   public native final void setShowPrintMargin(boolean show) /*-{
      this.setShowPrintMargin(show);
   }-*/;

   public native final void setPrintMarginColumn(int column) /*-{
      this.setPrintMarginColumn(column);
   }-*/;

   public native final boolean getHighlightActiveLine() /*-{
      return this.getHighlightActiveLine();
   }-*/;
   
   public native final void setHighlightActiveLine(boolean highlight) /*-{
      this.setHighlightActiveLine(highlight);
   }-*/;
   
   public native final void setHighlightGutterLine(boolean highlight) /*-{
      this.setHighlightGutterLine(highlight);
   }-*/;

   public native final void setHighlightSelectedWord(boolean highlight) /*-{
      this.setHighlightSelectedWord(highlight);
   }-*/;

   public native final boolean getReadOnly() /*-{
      return this.getReadOnly();
   }-*/;

   public native final void setReadOnly(boolean readOnly) /*-{
      this.setReadOnly(readOnly);
   }-*/;
   
   public native final void setCompletionOptions(boolean enabled,
                                                 boolean snippets,
                                                 boolean live,
                                                 int characterThreshold,
                                                 int delayMilliseconds) /*-{
      this.setOptions({
        enableBasicAutocompletion: enabled,
        enableSnippets: enabled && snippets,
        enableLiveAutocompletion: enabled && live,
        completionCharacterThreshold: characterThreshold,
        completionDelay: delayMilliseconds
      });
   }-*/;
   
   public native final void toggleCommentLines() /*-{
      this.toggleCommentLines();
   }-*/;

   public native final void focus() /*-{
      this.focus();
   }-*/;
   
   public native final boolean isFocused() /*-{
      return this.isFocused();
   }-*/;
   
   public native final boolean isRowFullyVisible(int row) /*-{
      return this.isRowFullyVisible(row);
   }-*/;

   public native final void blur() /*-{
      this.blur();
   }-*/;

   public native final void setKeyboardHandler(KeyboardHandler keyboardHandler) /*-{
      this.setKeyboardHandler(keyboardHandler);
   }-*/;
   
   public native final void addKeyboardHandler(KeyboardHandler keyboardHandler) /*-{
      this.keyBinding.addKeyboardHandler(keyboardHandler);
   }-*/;
   
   public native final boolean isVimInInsertMode() /*-{
      return this.state.cm.state.vim.insertMode;
   }-*/;

   public native final void onChange(CommandWithArg<AceDocumentChangeEventNative> command) /*-{
      this.getSession().on("change",
        $entry(function (arg) {
            command.@org.rstudio.core.client.CommandWithArg::execute(Ljava/lang/Object;)(arg);
        }));
   }-*/;

   public native final void onChangeFold(Command command) /*-{
      this.getSession().on("changeFold",
              $entry(function () {
                 command.@com.google.gwt.user.client.Command::execute()();
              }));
   }-*/;
   
   public native final <T> void onGutterMouseDown(CommandWithArg<T> command) /*-{
      this.on("guttermousedown",
         $entry(function (arg) {
            command.@org.rstudio.core.client.CommandWithArg::execute(Ljava/lang/Object;)(arg);
         }));         
   }-*/;

   public final HandlerRegistration delegateEventsTo(HasHandlers handlers)
   {
      final LinkedList<JavaScriptObject> handles = new LinkedList<JavaScriptObject>();
      handles.add(addDomListener(getTextInputElement(), "keydown", handlers));
      handles.add(addDomListener(getTextInputElement(), "keypress", handlers));
      handles.add(addDomListener(this.<Element>cast(), "focus", handlers));
      handles.add(addDomListener(this.<Element>cast(), "blur", handlers));

      return new HandlerRegistration()
      {
         public void removeHandler()
         {
            while (!handles.isEmpty())
               removeDomListener(handles.remove());
         }
      };
   }

   private native Element getTextInputElement() /*-{
      return this.textInput.getElement();
   }-*/;

   private native static JavaScriptObject addDomListener(
         Element element,
         String eventName,
         HasHandlers hasHandlers) /*-{
      var event = $wnd.require("ace/lib/event");
      var listener = $entry(function(e) {
         @com.google.gwt.event.dom.client.DomEvent::fireNativeEvent(Lcom/google/gwt/dom/client/NativeEvent;Lcom/google/gwt/event/shared/HasHandlers;Lcom/google/gwt/dom/client/Element;)(e, hasHandlers, element);
      }); 
      event.addListener(element, eventName, listener);
      return $entry(function() {
         event.removeListener(element, eventName, listener);
      });
   }-*/;

   private native static void removeDomListener(JavaScriptObject handle) /*-{
      handle();
   }-*/;

   public static native AceEditorNative createEditor(Element container) /*-{
      var require = $wnd.require;
      var loader = require("rstudio/loader");
      return loader.loadEditor(container);
   }-*/;
   
   public final native void manageDefaultKeybindings() /*-{
      // We bind 'Ctrl + Shift + M' to insert a magrittr shortcut on Windows
      delete this.commands.commandKeyBinding["ctrl-shift-m"];
      
      // We bind 'Ctrl + Shift + P' to run previous code on Windows
      delete this.commands.commandKeyBinding["ctrl-shift-p"];
   }-*/;

   public static <T> HandlerRegistration addEventListener(
         JavaScriptObject target,
         String event,
         CommandWithArg<T> command)
   {
      final JavaScriptObject functor = addEventListenerInternal(target,
                                                                event,
                                                                command);
      return new HandlerRegistration()
      {
         public void removeHandler()
         {
            invokeFunctor(functor);
         }
      };
   }

   private static native <T> JavaScriptObject addEventListenerInternal(
         JavaScriptObject target,
         String eventName,
         CommandWithArg<T> command) /*-{
      var callback = $entry(function(arg) {
         if (arg && arg.text)
            arg = arg.text;
         command.@org.rstudio.core.client.CommandWithArg::execute(Ljava/lang/Object;)(arg);
      });

      target.addEventListener(eventName, callback);
      return function() {
         target.removeEventListener(eventName, callback);
      };
   }-*/;

   private static native void invokeFunctor(JavaScriptObject functor) /*-{
      functor();
   }-*/;

   public final native void scrollToRow(int row) /*-{
      this.scrollToRow(row);
   }-*/;

   public final native void scrollToLine(int line, boolean center) /*-{
      this.scrollToLine(line, center);
   }-*/;
   
   public final native void jumpToMatching(boolean select, boolean expand) /*-{
      this.jumpToMatching(select, expand);
   }-*/;
   
   public native final void revealRange(Range range, boolean animate) /*-{
      this.revealRange(range, animate);
   }-*/;

   public final native void autoHeight() /*-{
      var editor = this;
      function updateEditorHeight() {
         editor.container.style.height = (Math.max(1, editor.getSession().getScreenLength()) * editor.renderer.lineHeight) + 'px';
         editor.resize();
         editor.renderer.scrollToY(0);
         editor.renderer.scrollToX(0);
      }
      if (!editor.autoHeightAttached) {
         editor.autoHeightAttached = true;
         editor.getSession().getDocument().on("change", updateEditorHeight);
         editor.renderer.$textLayer.on("changeCharacterSize", updateEditorHeight);
      }
      updateEditorHeight();
   }-*/;

   public final native void onCursorChange() /*-{
      this.onCursorChange();
   }-*/;

   public static native void setInsertMatching(boolean insertMatching) /*-{
      $wnd.require("mode/auto_brace_insert").setInsertMatching(insertMatching);
   }-*/;

   public static native void setVerticallyAlignFunctionArgs(
         boolean verticallyAlign) /*-{
      $wnd.require("mode/r_code_model").setVerticallyAlignFunctionArgs(verticallyAlign);
   }-*/;

   public final native int getFirstVisibleRow() /*-{
      return this.getFirstVisibleRow();
   }-*/;

   public final native int getLastVisibleRow() /*-{
      return this.getLastVisibleRow();
   }-*/;
   
   public final native int findAll(String needle) /*-{
      return this.findAll(needle);
   }-*/;
   
   public final native void insert(String text) /*-{
      var that = this;
      this.forEachSelection(function() {
         that.insert(text);
      });
   }-*/;
   
   public final native void moveCursorLeft(int times) /*-{
      var that = this;
      this.forEachSelection(function() {
         that.navigateLeft(times);
      });
   }-*/;
   
   public final native void moveCursorRight(int times) /*-{
      var that = this;
      this.forEachSelection(function() {
         that.navigateRight(times);
      });
   }-*/;
   
   public final native void expandSelectionLeft(int times) /*-{
      var that = this;
      this.forEachSelection(function() {
         var selection = that.getSelection();
         for (var i = 0; i < times; i++)
            selection.selectLeft();
      });
   }-*/;
   
   public final native void expandSelectionRight(int times) /*-{
      var that = this;
      this.forEachSelection(function() {
         var selection = that.getSelection();
         for (var i = 0; i < times; i++)
            selection.selectRight();
      });
   }-*/;
   
   public final native Position getCursorPosition() /*-{
      return this.getCursorPosition();
   }-*/;
   
   public final native void blockOutdent() /*-{
      return this.blockOutdent();
   }-*/;
   
}
