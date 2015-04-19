/*
 * rhtml.js
 *
 * Copyright (C) 2009-12 by RStudio, Inc.
 *
 * The Initial Developer of the Original Code is
 * Ajax.org B.V.
 * Portions created by the Initial Developer are Copyright (C) 2010
 * the Initial Developer. All Rights Reserved.
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

define("mode/rhtml", function(require, exports, module) {

var oop = require("ace/lib/oop");
var HtmlMode = require("ace/mode/html").Mode;
var Tokenizer = require("ace/tokenizer").Tokenizer;
var RHtmlHighlightRules = require("mode/rhtml_highlight_rules").RHtmlHighlightRules;
var SweaveBackgroundHighlighter = require("mode/sweave_background_highlighter").SweaveBackgroundHighlighter;
var RCodeModel = require("mode/r_code_model").RCodeModel;
var Utils = require("mode/utils");

var Mode = function(suppressHighlighting, session) {
   this.$session = session;
   this.$tokenizer = new Tokenizer(new RHtmlHighlightRules().getRules());

   this.codeModel = new RCodeModel(
      session,
      this.$tokenizer,
      /^r-/,
      /^<!--\s*begin.rcode\s*(.*)/,
      /^\s*end.rcode\s*-->/
   );
   
   this.foldingRules = this.codeModel;
   this.$sweaveBackgroundHighlighter = new SweaveBackgroundHighlighter(
         session,
         /^<!--\s*begin.rcode\s*(?:.*)/,
         /^\s*end.rcode\s*-->/,
         true);
};
oop.inherits(Mode, HtmlMode);

(function() {

   this.insertChunkInfo = {
      value: "<!--begin.rcode\n\nend.rcode-->\n",
      position: {row: 0, column: 15}
   };
    
   this.getLanguageMode = function(position)
   {
      var state = Utils.getPrimaryState(this.$session, position.row);
      return state.match(/^r-/) ? 'R' : 'HTML';
   };

   this.$getNextLineIndent = this.getNextLineIndent;
   this.getNextLineIndent = function(state, line, tab, row)
   {
      var mode = Utils.activeMode(state, "html");
      if (mode === "r")
         return this.codeModel.getNextLineIndent(state, line, tab, row);
      else
         return this.$getNextLineIndent(state, line, tab);
   };

}).call(Mode.prototype);

exports.Mode = Mode;
});
