/*
 * searchtools.js_t
 * ~~~~~~~~~~~~~~~~
 *
 * Sphinx JavaScript utilities for the full-text search.
 *
 * :copyright: Copyright 2007-2018 by the Sphinx team, see AUTHORS.
 * :license: BSD, see LICENSE for details.
 *
 */


/* Non-minified version JS is _stemmer.js if file is provided */ 

var JSX={};(function(h){function j(b,e){var a=function(){};a.prototype=e.prototype;var c=new a;for(var d in b){b[d].prototype=c}}function P(c,b){for(var a in b.prototype)if(b.prototype.hasOwnProperty(a))c.prototype[a]=b.prototype[a]}function e(a,b,d){function c(a,b,c){delete a[b];a[b]=c;return c}Object.defineProperty(a,b,{get:function(){return c(a,b,d())},set:function(d){c(a,b,d)},enumerable:true,configurable:true})}function O(a,b,c){return a[b]=a[b]/c|0}var u=parseInt;var v=parseFloat;function N(a){return a!==a}var x=isFinite;var y=encodeURIComponent;var z=decodeURIComponent;var B=encodeURI;var C=decodeURI;var E=Object.prototype.toString;var F=Object.prototype.hasOwnProperty;function i(){}h.require=function(b){var a=q[b];return a!==undefined?a:null};h.profilerIsRunning=function(){return i.getResults!=null};h.getProfileResults=function(){return(i.getResults||function(){return{}})()};h.postProfileResults=function(a,b){if(i.postResults==null)throw new Error('profiler has not been turned on');return i.postResults(a,b)};h.resetProfileResults=function(){if(i.resetResults==null)throw new Error('profiler has not been turned on');return i.resetResults()};h.DEBUG=false;function r(){};j([r],Error);function a(a,b,c){this.F=a.length;this.K=a;this.L=b;this.I=c;this.H=null;this.P=null};j([a],Object);function n(){};j([n],Object);function f(){var a;var b;var c;this.G={};a=this.D='';b=this._=0;c=this.A=a.length;this.E=0;this.B=b;this.C=c};j([f],n);function s(a,b){a.D=b.D;a._=b._;a.A=b.A;a.E=b.E;a.B=b.B;a.C=b.C};function k(b,d,c,e){var a;if(b._>=b.A){return false}a=b.D.charCodeAt(b._);if(a>e||a<c){return false}a-=c;if((d[a>>>3]&1<<(a&7))===0){return false}b._++;return true};function l(a,d,c,e){var b;if(a._>=a.A){return false}b=a.D.charCodeAt(a._);if(b>e||b<c){a._++;return true}b-=c;if((d[b>>>3]&1<<(b&7))===0){a._++;return true}return false};function o(f,m,p){var b;var d;var e;var n;var g;var k;var l;var i;var h;var c;var a;var j;var o;b=0;d=p;e=f._;n=f.A;g=0;k=0;l=false;while(true){i=b+(d-b>>>1);h=0;c=g<k?g:k;a=m[i];for(j=c;j<a.F;j++){if(e+c===n){h=-1;break}h=f.D.charCodeAt(e+c)-a.K.charCodeAt(j);if(h!==0){break}c++}if(h<0){d=i;k=c}else{b=i;g=c}if(d-b<=1){if(b>0){break}if(d===b){break}if(l){break}l=true}}while(true){a=m[b];if(g>=a.F){f._=e+a.F|0;if(a.H==null){return a.I}o=a.H(a.P);f._=e+a.F|0;if(o){return a.I}}b=a.L;if(b<0){return 0}}return-1};function d(d,m,p){var b;var g;var e;var n;var f;var k;var l;var i;var h;var c;var a;var j;var o;b=0;g=p;e=d._;n=d.E;f=0;k=0;l=false;while(true){i=b+(g-b>>1);h=0;c=f<k?f:k;a=m[i];for(j=a.F-1-c;j>=0;j--){if(e-c===n){h=-1;break}h=d.D.charCodeAt(e-1-c)-a.K.charCodeAt(j);if(h!==0){break}c++}if(h<0){g=i;k=c}else{b=i;f=c}if(g-b<=1){if(b>0){break}if(g===b){break}if(l){break}l=true}}while(true){a=m[b];if(f>=a.F){d._=e-a.F|0;if(a.H==null){return a.I}o=a.H(d);d._=e-a.F|0;if(o){return a.I}}b=a.L;if(b<0){return 0}}return-1};function A(a,b,d,e){var c;c=e.length-(d-b);a.D=a.D.slice(0,b)+e+a.D.slice(d);a.A+=c|0;if(a._>=d){a._+=c|0}else if(a._>b){a._=b}return c|0};function b(a,f){var b;var c;var d;var e;b=false;if((c=a.B)<0||c>(d=a.C)||d>(e=a.A)||e>a.D.length?false:true){A(a,a.B,a.C,f);b=true}return b};f.prototype.J=function(){return false};f.prototype.e=function(b){var a;var c;var d;var e;a=this.G['.'+b];if(a==null){c=this.D=b;d=this._=0;e=this.A=c.length;this.E=0;this.B=d;this.C=e;this.J();a=this.D;this.G['.'+b]=a}return a};f.prototype.stemWord=f.prototype.e;f.prototype.f=function(e){var d;var b;var c;var a;var f;var g;var h;d=[];for(b=0;b<e.length;b++){c=e[b];a=this.G['.'+c];if(a==null){f=this.D=c;g=this._=0;h=this.A=f.length;this.E=0;this.B=g;this.C=h;this.J();a=this.D;this.G['.'+c]=a}d.push(a)}return d};f.prototype.stemWords=f.prototype.f;function c(){f.call(this);this.I_p1=0};j([c],f);c.prototype.M=function(a){this.I_p1=a.I_p1;s(this,a)};c.prototype.copy_from=c.prototype.M;c.prototype.X=function(){var m;var b;var j;var d;var e;var a;var f;var g;var h;var n;var i;this.I_p1=this.A;d=true;b:while(d===true){d=false;m=this._;e=true;a:while(e===true){e=false;if(!k(this,c.g_v,97,252)){break a}c:while(true){b=this._;a=true;d:while(a===true){a=false;if(!l(this,c.g_v,97,252)){break d}this._=b;break c}n=this._=b;if(n>=this.A){break a}this._++}f=true;c:while(f===true){f=false;j=this._;g=true;d:while(g===true){g=false;if(o(this,c.a_0,8)===0){break d}break c}i=this._=j;if(i>=this.A){break a}this._++}this.I_p1=this._;break b}this._=m;if(!l(this,c.g_v,97,252)){return false}a:while(true){h=true;c:while(h===true){h=false;if(!k(this,c.g_v,97,252)){break c}break a}if(this._>=this.A){return false}this._++}this.I_p1=this._}return true};c.prototype.r_mark_regions=c.prototype.X;function D(a){var j;var d;var n;var e;var b;var f;var g;var h;var i;var p;var m;a.I_p1=a.A;e=true;b:while(e===true){e=false;j=a._;b=true;a:while(b===true){b=false;if(!k(a,c.g_v,97,252)){break a}c:while(true){d=a._;f=true;d:while(f===true){f=false;if(!l(a,c.g_v,97,252)){break d}a._=d;break c}p=a._=d;if(p>=a.A){break a}a._++}g=true;c:while(g===true){g=false;n=a._;h=true;d:while(h===true){h=false;if(o(a,c.a_0,8)===0){break d}break c}m=a._=n;if(m>=a.A){break a}a._++}a.I_p1=a._;break b}a._=j;if(!l(a,c.g_v,97,252)){return false}a:while(true){i=true;c:while(i===true){i=false;if(!k(a,c.g_v,97,252)){break c}break a}if(a._>=a.A){return false}a._++}a.I_p1=a._}return true};c.prototype.Q=function(){return!(this.I_p1<=this._)?false:true};c.prototype.r_R1=c.prototype.Q;c.prototype.d=function(){var a;var e;this.C=this._;a=d(this,c.a_1,2);if(a===0){return false}this.B=e=this._;if(!(!(this.I_p1<=e)?false:true)){return false}switch(a){case 0:return false;case 1:if(!b(this,'a')){return false}break;case 2:if(!b(this,'e')){return false}break}return true};c.prototype.r_v_ending=c.prototype.d;function p(a){var e;var f;a.C=a._;e=d(a,c.a_1,2);if(e===0){return false}a.B=f=a._;if(!(!(a.I_p1<=f)?false:true)){return false}switch(e){case 0:return false;case 1:if(!b(a,'a')){return false}break;case 2:if(!b(a,'e')){return false}break}return true};c.prototype.U=function(){var a;a=this.A-this._;if(d(this,c.a_2,23)===0){return false}this._=this.A-a;return true};c.prototype.r_double=c.prototype.U;function g(a){var b;b=a.A-a._;if(d(a,c.a_2,23)===0){return false}a._=a.A-b;return true};c.prototype.c=function(){var a;var c;var d;if(this._<=this.E){return false}this._--;this.C=c=this._;a=c-1|0;if(this.E>a||a>this.A){return false}d=this._=a;this.B=d;return!b(this,'')?false:true};c.prototype.r_undouble=c.prototype.c;function m(a){var c;var d;var e;if(a._<=a.E){return false}a._--;a.C=d=a._;c=d-1|0;if(a.E>c||c>a.A){return false}e=a._=c;a.B=e;return!b(a,'')?false:true};c.prototype.W=function(){var a;var e;this.C=this._;a=d(this,c.a_3,2);if(a===0){return false}this.B=e=this._;if(!(!(this.I_p1<=e)?false:true)){return false}switch(a){case 0:return false;case 1:if(!g(this)){return false}break;case 2:if(!g(this)){return false}break}return!b(this,'')?false:!m(this)?false:true};c.prototype.r_instrum=c.prototype.W;function H(a){var e;var f;a.C=a._;e=d(a,c.a_3,2);if(e===0){return false}a.B=f=a._;if(!(!(a.I_p1<=f)?false:true)){return false}switch(e){case 0:return false;case 1:if(!g(a)){return false}break;case 2:if(!g(a)){return false}break}return!b(a,'')?false:!m(a)?false:true};c.prototype.R=function(){var a;this.C=this._;if(d(this,c.a_4,44)===0){return false}this.B=a=this._;return!(!(this.I_p1<=a)?false:true)?false:!b(this,'')?false:!p(this)?false:true};c.prototype.r_case=c.prototype.R;function I(a){var e;a.C=a._;if(d(a,c.a_4,44)===0){return false}a.B=e=a._;return!(!(a.I_p1<=e)?false:true)?false:!b(a,'')?false:!p(a)?false:true};c.prototype.T=function(){var a;var e;this.C=this._;a=d(this,c.a_5,3);if(a===0){return false}this.B=e=this._;if(!(!(this.I_p1<=e)?false:true)){return false}switch(a){case 0:return false;case 1:if(!b(this,'e')){return false}break;case 2:if(!b(this,'a')){return false}break;case 3:if(!b(this,'a')){return false}break}return true};c.prototype.r_case_special=c.prototype.T;function J(a){var e;var f;a.C=a._;e=d(a,c.a_5,3);if(e===0){return false}a.B=f=a._;if(!(!(a.I_p1<=f)?false:true)){return false}switch(e){case 0:return false;case 1:if(!b(a,'e')){return false}break;case 2:if(!b(a,'a')){return false}break;case 3:if(!b(a,'a')){return false}break}return true};c.prototype.S=function(){var a;var e;this.C=this._;a=d(this,c.a_6,6);if(a===0){return false}this.B=e=this._;if(!(!(this.I_p1<=e)?false:true)){return false}switch(a){case 0:return false;case 1:if(!b(this,'')){return false}break;case 2:if(!b(this,'')){return false}break;case 3:if(!b(this,'a')){return false}break;case 4:if(!b(this,'e')){return false}break}return true};c.prototype.r_case_other=c.prototype.S;function K(a){var e;var f;a.C=a._;e=d(a,c.a_6,6);if(e===0){return false}a.B=f=a._;if(!(!(a.I_p1<=f)?false:true)){return false}switch(e){case 0:return false;case 1:if(!b(a,'')){return false}break;case 2:if(!b(a,'')){return false}break;case 3:if(!b(a,'a')){return false}break;case 4:if(!b(a,'e')){return false}break}return true};c.prototype.V=function(){var a;var e;this.C=this._;a=d(this,c.a_7,2);if(a===0){return false}this.B=e=this._;if(!(!(this.I_p1<=e)?false:true)){return false}switch(a){case 0:return false;case 1:if(!g(this)){return false}break;case 2:if(!g(this)){return false}break}return!b(this,'')?false:!m(this)?false:true};c.prototype.r_factive=c.prototype.V;function L(a){var e;var f;a.C=a._;e=d(a,c.a_7,2);if(e===0){return false}a.B=f=a._;if(!(!(a.I_p1<=f)?false:true)){return false}switch(e){case 0:return false;case 1:if(!g(a)){return false}break;case 2:if(!g(a)){return false}break}return!b(a,'')?false:!m(a)?false:true};c.prototype.a=function(){var a;var e;this.C=this._;a=d(this,c.a_8,7);if(a===0){return false}this.B=e=this._;if(!(!(this.I_p1<=e)?false:true)){return false}switch(a){case 0:return false;case 1:if(!b(this,'a')){return false}break;case 2:if(!b(this,'e')){return false}break;case 3:if(!b(this,'')){return false}break;case 4:if(!b(this,'')){return false}break;case 5:if(!b(this,'')){return false}break;case 6:if(!b(this,'')){return false}break;case 7:if(!b(this,'')){return false}break}return true};c.prototype.r_plural=c.prototype.a;function M(a){var e;var f;a.C=a._;e=d(a,c.a_8,7);if(e===0){return false}a.B=f=a._;if(!(!(a.I_p1<=f)?false:true)){return false}switch(e){case 0:return false;case 1:if(!b(a,'a')){return false}break;case 2:if(!b(a,'e')){return false}break;case 3:if(!b(a,'')){return false}break;case 4:if(!b(a,'')){return false}break;case 5:if(!b(a,'')){return false}break;case 6:if(!b(a,'')){return false}break;case 7:if(!b(a,'')){return false}break}return true};c.prototype.Y=function(){var a;var e;this.C=this._;a=d(this,c.a_9,12);if(a===0){return false}this.B=e=this._;if(!(!(this.I_p1<=e)?false:true)){return false}switch(a){case 0:return false;case 1:if(!b(this,'')){return false}break;case 2:if(!b(this,'e')){return false}break;case 3:if(!b(this,'a')){return false}break;case 4:if(!b(this,'')){return false}break;case 5:if(!b(this,'e')){return false}break;case 6:if(!b(this,'a')){return false}break;case 7:if(!b(this,'')){return false}break;case 8:if(!b(this,'e')){return false}break;case 9:if(!b(this,'')){return false}break}return true};c.prototype.r_owned=c.prototype.Y;function w(a){var e;var f;a.C=a._;e=d(a,c.a_9,12);if(e===0){return false}a.B=f=a._;if(!(!(a.I_p1<=f)?false:true)){return false}switch(e){case 0:return false;case 1:if(!b(a,'')){return false}break;case 2:if(!b(a,'e')){return false}break;case 3:if(!b(a,'a')){return false}break;case 4:if(!b(a,'')){return false}break;case 5:if(!b(a,'e')){return false}break;case 6:if(!b(a,'a')){return false}break;case 7:if(!b(a,'')){return false}break;case 8:if(!b(a,'e')){return false}break;case 9:if(!b(a,'')){return false}break}return true};c.prototype.b=function(){var a;var e;this.C=this._;a=d(this,c.a_10,31);if(a===0){return false}this.B=e=this._;if(!(!(this.I_p1<=e)?false:true)){return false}switch(a){case 0:return false;case 1:if(!b(this,'')){return false}break;case 2:if(!b(this,'a')){return false}break;case 3:if(!b(this,'e')){return false}break;case 4:if(!b(this,'')){return false}break;case 5:if(!b(this,'a')){return false}break;case 6:if(!b(this,'e')){return false}break;case 7:if(!b(this,'')){return false}break;case 8:if(!b(this,'')){return false}break;case 9:if(!b(this,'')){return false}break;case 10:if(!b(this,'a')){return false}break;case 11:if(!b(this,'e')){return false}break;case 12:if(!b(this,'')){return false}break;case 13:if(!b(this,'')){return false}break;case 14:if(!b(this,'a')){return false}break;case 15:if(!b(this,'e')){return false}break;case 16:if(!b(this,'')){return false}break;case 17:if(!b(this,'')){return false}break;case 18:if(!b(this,'')){return false}break;case 19:if(!b(this,'a')){return false}break;case 20:if(!b(this,'e')){return false}break}return true};c.prototype.r_sing_owner=c.prototype.b;function t(a){var e;var f;a.C=a._;e=d(a,c.a_10,31);if(e===0){return false}a.B=f=a._;if(!(!(a.I_p1<=f)?false:true)){return false}switch(e){case 0:return false;case 1:if(!b(a,'')){return false}break;case 2:if(!b(a,'a')){return false}break;case 3:if(!b(a,'e')){return false}break;case 4:if(!b(a,'')){return false}break;case 5:if(!b(a,'a')){return false}break;case 6:if(!b(a,'e')){return false}break;case 7:if(!b(a,'')){return false}break;case 8:if(!b(a,'')){return false}break;case 9:if(!b(a,'')){return false}break;case 10:if(!b(a,'a')){return false}break;case 11:if(!b(a,'e')){return false}break;case 12:if(!b(a,'')){return false}break;case 13:if(!b(a,'')){return false}break;case 14:if(!b(a,'a')){return false}break;case 15:if(!b(a,'e')){return false}break;case 16:if(!b(a,'')){return false}break;case 17:if(!b(a,'')){return false}break;case 18:if(!b(a,'')){return false}break;case 19:if(!b(a,'a')){return false}break;case 20:if(!b(a,'e')){return false}break}return true};c.prototype.Z=function(){var a;var e;this.C=this._;a=d(this,c.a_11,42);if(a===0){return false}this.B=e=this._;if(!(!(this.I_p1<=e)?false:true)){return false}switch(a){case 0:return false;case 1:if(!b(this,'')){return false}break;case 2:if(!b(this,'a')){return false}break;case 3:if(!b(this,'e')){return false}break;case 4:if(!b(this,'')){return false}break;case 5:if(!b(this,'')){return false}break;case 6:if(!b(this,'')){return false}break;case 7:if(!b(this,'a')){return false}break;case 8:if(!b(this,'e')){return false}break;case 9:if(!b(this,'')){return false}break;case 10:if(!b(this,'')){return false}break;case 11:if(!b(this,'')){return false}break;case 12:if(!b(this,'a')){return false}break;case 13:if(!b(this,'e')){return false}break;case 14:if(!b(this,'')){return false}break;case 15:if(!b(this,'')){return false}break;case 16:if(!b(this,'')){return false}break;case 17:if(!b(this,'')){return false}break;case 18:if(!b(this,'a')){return false}break;case 19:if(!b(this,'e')){return false}break;case 20:if(!b(this,'')){return false}break;case 21:if(!b(this,'')){return false}break;case 22:if(!b(this,'a')){return false}break;case 23:if(!b(this,'e')){return false}break;case 24:if(!b(this,'')){return false}break;case 25:if(!b(this,'')){return false}break;case 26:if(!b(this,'')){return false}break;case 27:if(!b(this,'a')){return false}break;case 28:if(!b(this,'e')){return false}break;case 29:if(!b(this,'')){return false}break}return true};c.prototype.r_plur_owner=c.prototype.Z;function G(a){var e;var f;a.C=a._;e=d(a,c.a_11,42);if(e===0){return false}a.B=f=a._;if(!(!(a.I_p1<=f)?false:true)){return false}switch(e){case 0:return false;case 1:if(!b(a,'')){return false}break;case 2:if(!b(a,'a')){return false}break;case 3:if(!b(a,'e')){return false}break;case 4:if(!b(a,'')){return false}break;case 5:if(!b(a,'')){return false}break;case 6:if(!b(a,'')){return false}break;case 7:if(!b(a,'a')){return false}break;case 8:if(!b(a,'e')){return false}break;case 9:if(!b(a,'')){return false}break;case 10:if(!b(a,'')){return false}break;case 11:if(!b(a,'')){return false}break;case 12:if(!b(a,'a')){return false}break;case 13:if(!b(a,'e')){return false}break;case 14:if(!b(a,'')){return false}break;case 15:if(!b(a,'')){return false}break;case 16:if(!b(a,'')){return false}break;case 17:if(!b(a,'')){return false}break;case 18:if(!b(a,'a')){return false}break;case 19:if(!b(a,'e')){return false}break;case 20:if(!b(a,'')){return false}break;case 21:if(!b(a,'')){return false}break;case 22:if(!b(a,'a')){return false}break;case 23:if(!b(a,'e')){return false}break;case 24:if(!b(a,'')){return false}break;case 25:if(!b(a,'')){return false}break;case 26:if(!b(a,'')){return false}break;case 27:if(!b(a,'a')){return false}break;case 28:if(!b(a,'e')){return false}break;case 29:if(!b(a,'')){return false}break}return true};c.prototype.J=function(){var s;var l;var m;var n;var o;var p;var q;var r;var u;var b;var c;var d;var e;var f;var g;var h;var i;var a;var j;var v;var x;var y;var z;var A;var B;var C;var E;var F;var N;var O;var P;var Q;var R;var S;var T;var k;s=this._;b=true;a:while(b===true){b=false;if(!D(this)){break a}}v=this._=s;this.E=v;y=this._=x=this.A;l=x-y;c=true;a:while(c===true){c=false;if(!H(this)){break a}}A=this._=(z=this.A)-l;m=z-A;d=true;a:while(d===true){d=false;if(!I(this)){break a}}C=this._=(B=this.A)-m;n=B-C;e=true;a:while(e===true){e=false;if(!J(this)){break a}}F=this._=(E=this.A)-n;o=E-F;f=true;a:while(f===true){f=false;if(!K(this)){break a}}O=this._=(N=this.A)-o;p=N-O;g=true;a:while(g===true){g=false;if(!L(this)){break a}}Q=this._=(P=this.A)-p;q=P-Q;h=true;a:while(h===true){h=false;if(!w(this)){break a}}S=this._=(R=this.A)-q;r=R-S;i=true;a:while(i===true){i=false;if(!t(this)){break a}}k=this._=(T=this.A)-r;u=T-k;a=true;a:while(a===true){a=false;if(!G(this)){break a}}this._=this.A-u;j=true;a:while(j===true){j=false;if(!M(this)){break a}}this._=this.E;return true};c.prototype.stem=c.prototype.J;c.prototype.N=function(a){return a instanceof c};c.prototype.equals=c.prototype.N;c.prototype.O=function(){var c;var a;var b;var d;c='HungarianStemmer';a=0;for(b=0;b<c.length;b++){d=c.charCodeAt(b);a=(a<<5)-a+d;a=a&a}return a|0};c.prototype.hashCode=c.prototype.O;c.serialVersionUID=1;e(c,'methodObject',function(){return new c});e(c,'a_0',function(){return[new a('cs',-1,-1),new a('dzs',-1,-1),new a('gy',-1,-1),new a('ly',-1,-1),new a('ny',-1,-1),new a('sz',-1,-1),new a('ty',-1,-1),new a('zs',-1,-1)]});e(c,'a_1',function(){return[new a('á',-1,1),new a('é',-1,2)]});e(c,'a_2',function(){return[new a('bb',-1,-1),new a('cc',-1,-1),new a('dd',-1,-1),new a('ff',-1,-1),new a('gg',-1,-1),new a('jj',-1,-1),new a('kk',-1,-1),new a('ll',-1,-1),new a('mm',-1,-1),new a('nn',-1,-1),new a('pp',-1,-1),new a('rr',-1,-1),new a('ccs',-1,-1),new a('ss',-1,-1),new a('zzs',-1,-1),new a('tt',-1,-1),new a('vv',-1,-1),new a('ggy',-1,-1),new a('lly',-1,-1),new a('nny',-1,-1),new a('tty',-1,-1),new a('ssz',-1,-1),new a('zz',-1,-1)]});e(c,'a_3',function(){return[new a('al',-1,1),new a('el',-1,2)]});e(c,'a_4',function(){return[new a('ba',-1,-1),new a('ra',-1,-1),new a('be',-1,-1),new a('re',-1,-1),new a('ig',-1,-1),new a('nak',-1,-1),new a('nek',-1,-1),new a('val',-1,-1),new a('vel',-1,-1),new a('ul',-1,-1),new a('nál',-1,-1),new a('nél',-1,-1),new a('ból',-1,-1),new a('ról',-1,-1),new a('tól',-1,-1),new a('bõl',-1,-1),new a('rõl',-1,-1),new a('tõl',-1,-1),new a('ül',-1,-1),new a('n',-1,-1),new a('an',19,-1),new a('ban',20,-1),new a('en',19,-1),new a('ben',22,-1),new a('képpen',22,-1),new a('on',19,-1),new a('ön',19,-1),new a('képp',-1,-1),new a('kor',-1,-1),new a('t',-1,-1),new a('at',29,-1),new a('et',29,-1),new a('ként',29,-1),new a('anként',32,-1),new a('enként',32,-1),new a('onként',32,-1),new a('ot',29,-1),new a('ért',29,-1),new a('öt',29,-1),new a('hez',-1,-1),new a('hoz',-1,-1),new a('höz',-1,-1),new a('vá',-1,-1),new a('vé',-1,-1)]});e(c,'a_5',function(){return[new a('án',-1,2),new a('én',-1,1),new a('ánként',-1,3)]});e(c,'a_6',function(){return[new a('stul',-1,2),new a('astul',0,1),new a('ástul',0,3),new a('stül',-1,2),new a('estül',3,1),new a('éstül',3,4)]});e(c,'a_7',function(){return[new a('á',-1,1),new a('é',-1,2)]});e(c,'a_8',function(){return[new a('k',-1,7),new a('ak',0,4),new a('ek',0,6),new a('ok',0,5),new a('ák',0,1),new a('ék',0,2),new a('ök',0,3)]});e(c,'a_9',function(){return[new a('éi',-1,7),new a('áéi',0,6),new a('ééi',0,5),new a('é',-1,9),new a('ké',3,4),new a('aké',4,1),new a('eké',4,1),new a('oké',4,1),new a('áké',4,3),new a('éké',4,2),new a('öké',4,1),new a('éé',3,8)]});e(c,'a_10',function(){return[new a('a',-1,18),new a('ja',0,17),new a('d',-1,16),new a('ad',2,13),new a('ed',2,13),new a('od',2,13),new a('ád',2,14),new a('éd',2,15),new a('öd',2,13),new a('e',-1,18),new a('je',9,17),new a('nk',-1,4),new a('unk',11,1),new a('ánk',11,2),new a('énk',11,3),new a('ünk',11,1),new a('uk',-1,8),new a('juk',16,7),new a('ájuk',17,5),new a('ük',-1,8),new a('jük',19,7),new a('éjük',20,6),new a('m',-1,12),new a('am',22,9),new a('em',22,9),new a('om',22,9),new a('ám',22,10),new a('ém',22,11),new a('o',-1,18),new a('á',-1,19),new a('é',-1,20)]});e(c,'a_11',function(){return[new a('id',-1,10),new a('aid',0,9),new a('jaid',1,6),new a('eid',0,9),new a('jeid',3,6),new a('áid',0,7),new a('éid',0,8),new a('i',-1,15),new a('ai',7,14),new a('jai',8,11),new a('ei',7,14),new a('jei',10,11),new a('ái',7,12),new a('éi',7,13),new a('itek',-1,24),new a('eitek',14,21),new a('jeitek',15,20),new a('éitek',14,23),new a('ik',-1,29),new a('aik',18,26),new a('jaik',19,25),new a('eik',18,26),new a('jeik',21,25),new a('áik',18,27),new a('éik',18,28),new a('ink',-1,20),new a('aink',25,17),new a('jaink',26,16),new a('eink',25,17),new a('jeink',28,16),new a('áink',25,18),new a('éink',25,19),new a('aitok',-1,21),new a('jaitok',32,20),new a('áitok',-1,22),new a('im',-1,5),new a('aim',35,4),new a('jaim',36,1),new a('eim',35,4),new a('jeim',38,1),new a('áim',35,2),new a('éim',35,3)]});e(c,'g_v',function(){return[17,65,16,0,0,0,0,0,0,0,0,0,0,0,0,0,1,17,52,14]});var q={'src/stemmer.jsx':{Stemmer:n},'src/hungarian-stemmer.jsx':{HungarianStemmer:c}}}(JSX))
var Stemmer = JSX.require("src/hungarian-stemmer.jsx").HungarianStemmer;



/**
 * Simple result scoring code.
 */
var Scorer = {
  // Implement the following function to further tweak the score for each result
  // The function takes a result array [filename, title, anchor, descr, score]
  // and returns the new score.
  /*
  score: function(result) {
    return result[4];
  },
  */

  // query matches the full name of an object
  objNameMatch: 11,
  // or matches in the last dotted part of the object name
  objPartialMatch: 6,
  // Additive scores depending on the priority of the object
  objPrio: {0:  15,   // used to be importantResults
            1:  5,   // used to be objectResults
            2: -5},  // used to be unimportantResults
  //  Used when the priority is not in the mapping.
  objPrioDefault: 0,

  // query found in title
  title: 15,
  // query found in terms
  term: 5
};





var splitChars = (function() {
    var result = {};
    var singles = [96, 180, 187, 191, 215, 247, 749, 885, 903, 907, 909, 930, 1014, 1648,
         1748, 1809, 2416, 2473, 2481, 2526, 2601, 2609, 2612, 2615, 2653, 2702,
         2706, 2729, 2737, 2740, 2857, 2865, 2868, 2910, 2928, 2948, 2961, 2971,
         2973, 3085, 3089, 3113, 3124, 3213, 3217, 3241, 3252, 3295, 3341, 3345,
         3369, 3506, 3516, 3633, 3715, 3721, 3736, 3744, 3748, 3750, 3756, 3761,
         3781, 3912, 4239, 4347, 4681, 4695, 4697, 4745, 4785, 4799, 4801, 4823,
         4881, 5760, 5901, 5997, 6313, 7405, 8024, 8026, 8028, 8030, 8117, 8125,
         8133, 8181, 8468, 8485, 8487, 8489, 8494, 8527, 11311, 11359, 11687, 11695,
         11703, 11711, 11719, 11727, 11735, 12448, 12539, 43010, 43014, 43019, 43587,
         43696, 43713, 64286, 64297, 64311, 64317, 64319, 64322, 64325, 65141];
    var i, j, start, end;
    for (i = 0; i < singles.length; i++) {
        result[singles[i]] = true;
    }
    var ranges = [[0, 47], [58, 64], [91, 94], [123, 169], [171, 177], [182, 184], [706, 709],
         [722, 735], [741, 747], [751, 879], [888, 889], [894, 901], [1154, 1161],
         [1318, 1328], [1367, 1368], [1370, 1376], [1416, 1487], [1515, 1519], [1523, 1568],
         [1611, 1631], [1642, 1645], [1750, 1764], [1767, 1773], [1789, 1790], [1792, 1807],
         [1840, 1868], [1958, 1968], [1970, 1983], [2027, 2035], [2038, 2041], [2043, 2047],
         [2070, 2073], [2075, 2083], [2085, 2087], [2089, 2307], [2362, 2364], [2366, 2383],
         [2385, 2391], [2402, 2405], [2419, 2424], [2432, 2436], [2445, 2446], [2449, 2450],
         [2483, 2485], [2490, 2492], [2494, 2509], [2511, 2523], [2530, 2533], [2546, 2547],
         [2554, 2564], [2571, 2574], [2577, 2578], [2618, 2648], [2655, 2661], [2672, 2673],
         [2677, 2692], [2746, 2748], [2750, 2767], [2769, 2783], [2786, 2789], [2800, 2820],
         [2829, 2830], [2833, 2834], [2874, 2876], [2878, 2907], [2914, 2917], [2930, 2946],
         [2955, 2957], [2966, 2968], [2976, 2978], [2981, 2983], [2987, 2989], [3002, 3023],
         [3025, 3045], [3059, 3076], [3130, 3132], [3134, 3159], [3162, 3167], [3170, 3173],
         [3184, 3191], [3199, 3204], [3258, 3260], [3262, 3293], [3298, 3301], [3312, 3332],
         [3386, 3388], [3390, 3423], [3426, 3429], [3446, 3449], [3456, 3460], [3479, 3481],
         [3518, 3519], [3527, 3584], [3636, 3647], [3655, 3663], [3674, 3712], [3717, 3718],
         [3723, 3724], [3726, 3731], [3752, 3753], [3764, 3772], [3774, 3775], [3783, 3791],
         [3802, 3803], [3806, 3839], [3841, 3871], [3892, 3903], [3949, 3975], [3980, 4095],
         [4139, 4158], [4170, 4175], [4182, 4185], [4190, 4192], [4194, 4196], [4199, 4205],
         [4209, 4212], [4226, 4237], [4250, 4255], [4294, 4303], [4349, 4351], [4686, 4687],
         [4702, 4703], [4750, 4751], [4790, 4791], [4806, 4807], [4886, 4887], [4955, 4968],
         [4989, 4991], [5008, 5023], [5109, 5120], [5741, 5742], [5787, 5791], [5867, 5869],
         [5873, 5887], [5906, 5919], [5938, 5951], [5970, 5983], [6001, 6015], [6068, 6102],
         [6104, 6107], [6109, 6111], [6122, 6127], [6138, 6159], [6170, 6175], [6264, 6271],
         [6315, 6319], [6390, 6399], [6429, 6469], [6510, 6511], [6517, 6527], [6572, 6592],
         [6600, 6607], [6619, 6655], [6679, 6687], [6741, 6783], [6794, 6799], [6810, 6822],
         [6824, 6916], [6964, 6980], [6988, 6991], [7002, 7042], [7073, 7085], [7098, 7167],
         [7204, 7231], [7242, 7244], [7294, 7400], [7410, 7423], [7616, 7679], [7958, 7959],
         [7966, 7967], [8006, 8007], [8014, 8015], [8062, 8063], [8127, 8129], [8141, 8143],
         [8148, 8149], [8156, 8159], [8173, 8177], [8189, 8303], [8306, 8307], [8314, 8318],
         [8330, 8335], [8341, 8449], [8451, 8454], [8456, 8457], [8470, 8472], [8478, 8483],
         [8506, 8507], [8512, 8516], [8522, 8525], [8586, 9311], [9372, 9449], [9472, 10101],
         [10132, 11263], [11493, 11498], [11503, 11516], [11518, 11519], [11558, 11567],
         [11622, 11630], [11632, 11647], [11671, 11679], [11743, 11822], [11824, 12292],
         [12296, 12320], [12330, 12336], [12342, 12343], [12349, 12352], [12439, 12444],
         [12544, 12548], [12590, 12592], [12687, 12689], [12694, 12703], [12728, 12783],
         [12800, 12831], [12842, 12880], [12896, 12927], [12938, 12976], [12992, 13311],
         [19894, 19967], [40908, 40959], [42125, 42191], [42238, 42239], [42509, 42511],
         [42540, 42559], [42592, 42593], [42607, 42622], [42648, 42655], [42736, 42774],
         [42784, 42785], [42889, 42890], [42893, 43002], [43043, 43055], [43062, 43071],
         [43124, 43137], [43188, 43215], [43226, 43249], [43256, 43258], [43260, 43263],
         [43302, 43311], [43335, 43359], [43389, 43395], [43443, 43470], [43482, 43519],
         [43561, 43583], [43596, 43599], [43610, 43615], [43639, 43641], [43643, 43647],
         [43698, 43700], [43703, 43704], [43710, 43711], [43715, 43738], [43742, 43967],
         [44003, 44015], [44026, 44031], [55204, 55215], [55239, 55242], [55292, 55295],
         [57344, 63743], [64046, 64047], [64110, 64111], [64218, 64255], [64263, 64274],
         [64280, 64284], [64434, 64466], [64830, 64847], [64912, 64913], [64968, 65007],
         [65020, 65135], [65277, 65295], [65306, 65312], [65339, 65344], [65371, 65381],
         [65471, 65473], [65480, 65481], [65488, 65489], [65496, 65497]];
    for (i = 0; i < ranges.length; i++) {
        start = ranges[i][0];
        end = ranges[i][1];
        for (j = start; j <= end; j++) {
            result[j] = true;
        }
    }
    return result;
})();

function splitQuery(query) {
    var result = [];
    var start = -1;
    for (var i = 0; i < query.length; i++) {
        if (splitChars[query.charCodeAt(i)]) {
            if (start !== -1) {
                result.push(query.slice(start, i));
                start = -1;
            }
        } else if (start === -1) {
            start = i;
        }
    }
    if (start !== -1) {
        result.push(query.slice(start));
    }
    return result;
}




/**
 * Search Module
 */
var Search = {

  _index : null,
  _queued_query : null,
  _pulse_status : -1,

  init : function() {
      var params = $.getQueryParameters();
      if (params.q) {
          var query = params.q[0];
          $('input[name="q"]')[0].value = query;
          this.performSearch(query);
      }
  },

  loadIndex : function(url) {
    $.ajax({type: "GET", url: url, data: null,
            dataType: "script", cache: true,
            complete: function(jqxhr, textstatus) {
              if (textstatus != "success") {
                document.getElementById("searchindexloader").src = url;
              }
            }});
  },

  setIndex : function(index) {
    var q;
    this._index = index;
    if ((q = this._queued_query) !== null) {
      this._queued_query = null;
      Search.query(q);
    }
  },

  hasIndex : function() {
      return this._index !== null;
  },

  deferQuery : function(query) {
      this._queued_query = query;
  },

  stopPulse : function() {
      this._pulse_status = 0;
  },

  startPulse : function() {
    if (this._pulse_status >= 0)
        return;
    function pulse() {
      var i;
      Search._pulse_status = (Search._pulse_status + 1) % 4;
      var dotString = '';
      for (i = 0; i < Search._pulse_status; i++)
        dotString += '.';
      Search.dots.text(dotString);
      if (Search._pulse_status > -1)
        window.setTimeout(pulse, 500);
    }
    pulse();
  },

  /**
   * perform a search for something (or wait until index is loaded)
   */
  performSearch : function(query) {
    // create the required interface elements
    this.out = $('#search-results');
    this.title = $('<h2>' + _('Searching') + '</h2>').appendTo(this.out);
    this.dots = $('<span></span>').appendTo(this.title);
    this.status = $('<p style="display: none"></p>').appendTo(this.out);
    this.output = $('<ul class="search"/>').appendTo(this.out);

    $('#search-progress').text(_('Preparing search...'));
    this.startPulse();

    // index already loaded, the browser was quick!
    if (this.hasIndex())
      this.query(query);
    else
      this.deferQuery(query);
  },

  /**
   * execute search (requires search index to be loaded)
   */
  query : function(query) {
    var i;
    var stopwords = ["a","abban","ahhoz","ahogy","ahol","aki","akik","akkor","alatt","amely","amelyek","amelyekben","amelyeket","amelyet","amelynek","ami","amikor","amit","amolyan","am\u00edg","annak","arra","arr\u00f3l","az","azok","azon","azonban","azt","azt\u00e1n","azut\u00e1n","azzal","az\u00e9rt","be","bel\u00fcl","benne","b\u00e1r","cikk","cikkek","cikkeket","csak","de","e","ebben","eddig","egy","egyes","egyetlen","egyik","egyre","egy\u00e9b","eg\u00e9sz","ehhez","ekkor","el","ellen","els\u0151","el\u00e9g","el\u0151","el\u0151sz\u00f6r","el\u0151tt","emilyen","ennek","erre","ez","ezek","ezen","ezt","ezzel","ez\u00e9rt","fel","fel\u00e9","hanem","hiszen","hogy","hogyan","igen","ill","ill.","illetve","ilyen","ilyenkor","ism\u00e9t","ison","itt","jobban","j\u00f3","j\u00f3l","kell","kellett","keress\u00fcnk","kereszt\u00fcl","ki","k\u00edv\u00fcl","k\u00f6z\u00f6tt","k\u00f6z\u00fcl","legal\u00e1bb","legyen","lehet","lehetett","lenne","lenni","lesz","lett","maga","mag\u00e1t","majd","meg","mellett","mely","melyek","mert","mi","mikor","milyen","minden","mindenki","mindent","mindig","mint","mintha","mit","mivel","mi\u00e9rt","most","m\u00e1r","m\u00e1s","m\u00e1sik","m\u00e9g","m\u00edg","nagy","nagyobb","nagyon","ne","nekem","neki","nem","nincs","n\u00e9ha","n\u00e9h\u00e1ny","n\u00e9lk\u00fcl","olyan","ott","pedig","persze","r\u00e1","s","saj\u00e1t","sem","semmi","sok","sokat","sokkal","szemben","szerint","szinte","sz\u00e1m\u00e1ra","tal\u00e1n","teh\u00e1t","teljes","tov\u00e1bb","tov\u00e1bb\u00e1","t\u00f6bb","ugyanis","utols\u00f3","ut\u00e1n","ut\u00e1na","vagy","vagyis","vagyok","valaki","valami","valamint","val\u00f3","van","vannak","vele","vissza","viszont","volna","volt","voltak","voltam","voltunk","\u00e1ltal","\u00e1ltal\u00e1ban","\u00e1t","\u00e9n","\u00e9ppen","\u00e9s","\u00edgy","\u00f6ssze","\u00fagy","\u00faj","\u00fajabb","\u00fajra","\u0151","\u0151k","\u0151ket"];

    // stem the searchterms and add them to the correct list
    var stemmer = new Stemmer();
    var searchterms = [];
    var excluded = [];
    var hlterms = [];
    var tmp = splitQuery(query);
    var objectterms = [];
    for (i = 0; i < tmp.length; i++) {
      if (tmp[i] !== "") {
          objectterms.push(tmp[i].toLowerCase());
      }

      if ($u.indexOf(stopwords, tmp[i].toLowerCase()) != -1 || tmp[i].match(/^\d+$/) ||
          tmp[i] === "") {
        // skip this "word"
        continue;
      }
      // stem the word
      var word = stemmer.stemWord(tmp[i].toLowerCase());
      // prevent stemmer from cutting word smaller than two chars
      if(word.length < 3 && tmp[i].length >= 3) {
        word = tmp[i];
      }
      var toAppend;
      // select the correct list
      if (word[0] == '-') {
        toAppend = excluded;
        word = word.substr(1);
      }
      else {
        toAppend = searchterms;
        hlterms.push(tmp[i].toLowerCase());
      }
      // only add if not already in the list
      if (!$u.contains(toAppend, word))
        toAppend.push(word);
    }
    var highlightstring = '?highlight=' + $.urlencode(hlterms.join(" "));

    // console.debug('SEARCH: searching for:');
    // console.info('required: ', searchterms);
    // console.info('excluded: ', excluded);

    // prepare search
    var terms = this._index.terms;
    var titleterms = this._index.titleterms;

    // array of [filename, title, anchor, descr, score]
    var results = [];
    $('#search-progress').empty();

    // lookup as object
    for (i = 0; i < objectterms.length; i++) {
      var others = [].concat(objectterms.slice(0, i),
                             objectterms.slice(i+1, objectterms.length));
      results = results.concat(this.performObjectSearch(objectterms[i], others));
    }

    // lookup as search terms in fulltext
    results = results.concat(this.performTermsSearch(searchterms, excluded, terms, titleterms));

    // let the scorer override scores with a custom scoring function
    if (Scorer.score) {
      for (i = 0; i < results.length; i++)
        results[i][4] = Scorer.score(results[i]);
    }

    // now sort the results by score (in opposite order of appearance, since the
    // display function below uses pop() to retrieve items) and then
    // alphabetically
    results.sort(function(a, b) {
      var left = a[4];
      var right = b[4];
      if (left > right) {
        return 1;
      } else if (left < right) {
        return -1;
      } else {
        // same score: sort alphabetically
        left = a[1].toLowerCase();
        right = b[1].toLowerCase();
        return (left > right) ? -1 : ((left < right) ? 1 : 0);
      }
    });

    // for debugging
    //Search.lastresults = results.slice();  // a copy
    //console.info('search results:', Search.lastresults);

    // print the results
    var resultCount = results.length;
    function displayNextItem() {
      // results left, load the summary and display it
      if (results.length) {
        var item = results.pop();
        var listItem = $('<li style="display:none"></li>');
        if (DOCUMENTATION_OPTIONS.FILE_SUFFIX === '') {
          // dirhtml builder
          var dirname = item[0] + '/';
          if (dirname.match(/\/index\/$/)) {
            dirname = dirname.substring(0, dirname.length-6);
          } else if (dirname == 'index/') {
            dirname = '';
          }
          listItem.append($('<a/>').attr('href',
            DOCUMENTATION_OPTIONS.URL_ROOT + dirname +
            highlightstring + item[2]).html(item[1]));
        } else {
          // normal html builders
          listItem.append($('<a/>').attr('href',
            item[0] + DOCUMENTATION_OPTIONS.FILE_SUFFIX +
            highlightstring + item[2]).html(item[1]));
        }
        if (item[3]) {
          listItem.append($('<span> (' + item[3] + ')</span>'));
          Search.output.append(listItem);
          listItem.slideDown(5, function() {
            displayNextItem();
          });
        } else if (DOCUMENTATION_OPTIONS.HAS_SOURCE) {
          var suffix = DOCUMENTATION_OPTIONS.SOURCELINK_SUFFIX;
          if (suffix === undefined) {
            suffix = '.txt';
          }
          $.ajax({url: DOCUMENTATION_OPTIONS.URL_ROOT + '_sources/' + item[5] + (item[5].slice(-suffix.length) === suffix ? '' : suffix),
                  dataType: "text",
                  complete: function(jqxhr, textstatus) {
                    var data = jqxhr.responseText;
                    if (data !== '' && data !== undefined) {
                      listItem.append(Search.makeSearchSummary(data, searchterms, hlterms));
                    }
                    Search.output.append(listItem);
                    listItem.slideDown(5, function() {
                      displayNextItem();
                    });
                  }});
        } else {
          // no source available, just display title
          Search.output.append(listItem);
          listItem.slideDown(5, function() {
            displayNextItem();
          });
        }
      }
      // search finished, update title and status message
      else {
        Search.stopPulse();
        Search.title.text(_('Search Results'));
        if (!resultCount)
          Search.status.text(_('Your search did not match any documents. Please make sure that all words are spelled correctly and that you\'ve selected enough categories.'));
        else
            Search.status.text(_('Search finished, found %s page(s) matching the search query.').replace('%s', resultCount));
        Search.status.fadeIn(500);
      }
    }
    displayNextItem();
  },

  /**
   * search for object names
   */
  performObjectSearch : function(object, otherterms) {
    var filenames = this._index.filenames;
    var docnames = this._index.docnames;
    var objects = this._index.objects;
    var objnames = this._index.objnames;
    var titles = this._index.titles;

    var i;
    var results = [];

    for (var prefix in objects) {
      for (var name in objects[prefix]) {
        var fullname = (prefix ? prefix + '.' : '') + name;
        if (fullname.toLowerCase().indexOf(object) > -1) {
          var score = 0;
          var parts = fullname.split('.');
          // check for different match types: exact matches of full name or
          // "last name" (i.e. last dotted part)
          if (fullname == object || parts[parts.length - 1] == object) {
            score += Scorer.objNameMatch;
          // matches in last name
          } else if (parts[parts.length - 1].indexOf(object) > -1) {
            score += Scorer.objPartialMatch;
          }
          var match = objects[prefix][name];
          var objname = objnames[match[1]][2];
          var title = titles[match[0]];
          // If more than one term searched for, we require other words to be
          // found in the name/title/description
          if (otherterms.length > 0) {
            var haystack = (prefix + ' ' + name + ' ' +
                            objname + ' ' + title).toLowerCase();
            var allfound = true;
            for (i = 0; i < otherterms.length; i++) {
              if (haystack.indexOf(otherterms[i]) == -1) {
                allfound = false;
                break;
              }
            }
            if (!allfound) {
              continue;
            }
          }
          var descr = objname + _(', in ') + title;

          var anchor = match[3];
          if (anchor === '')
            anchor = fullname;
          else if (anchor == '-')
            anchor = objnames[match[1]][1] + '-' + fullname;
          // add custom score for some objects according to scorer
          if (Scorer.objPrio.hasOwnProperty(match[2])) {
            score += Scorer.objPrio[match[2]];
          } else {
            score += Scorer.objPrioDefault;
          }
          results.push([docnames[match[0]], fullname, '#'+anchor, descr, score, filenames[match[0]]]);
        }
      }
    }

    return results;
  },

  /**
   * search for full-text terms in the index
   */
  performTermsSearch : function(searchterms, excluded, terms, titleterms) {
    var docnames = this._index.docnames;
    var filenames = this._index.filenames;
    var titles = this._index.titles;

    var i, j, file;
    var fileMap = {};
    var scoreMap = {};
    var results = [];

    // perform the search on the required terms
    for (i = 0; i < searchterms.length; i++) {
      var word = searchterms[i];
      var files = [];
      var _o = [
        {files: terms[word], score: Scorer.term},
        {files: titleterms[word], score: Scorer.title}
      ];

      // no match but word was a required one
      if ($u.every(_o, function(o){return o.files === undefined;})) {
        break;
      }
      // found search word in contents
      $u.each(_o, function(o) {
        var _files = o.files;
        if (_files === undefined)
          return

        if (_files.length === undefined)
          _files = [_files];
        files = files.concat(_files);

        // set score for the word in each file to Scorer.term
        for (j = 0; j < _files.length; j++) {
          file = _files[j];
          if (!(file in scoreMap))
            scoreMap[file] = {}
          scoreMap[file][word] = o.score;
        }
      });

      // create the mapping
      for (j = 0; j < files.length; j++) {
        file = files[j];
        if (file in fileMap)
          fileMap[file].push(word);
        else
          fileMap[file] = [word];
      }
    }

    // now check if the files don't contain excluded terms
    for (file in fileMap) {
      var valid = true;

      // check if all requirements are matched
      if (fileMap[file].length != searchterms.length)
          continue;

      // ensure that none of the excluded terms is in the search result
      for (i = 0; i < excluded.length; i++) {
        if (terms[excluded[i]] == file ||
            titleterms[excluded[i]] == file ||
            $u.contains(terms[excluded[i]] || [], file) ||
            $u.contains(titleterms[excluded[i]] || [], file)) {
          valid = false;
          break;
        }
      }

      // if we have still a valid result we can add it to the result list
      if (valid) {
        // select one (max) score for the file.
        // for better ranking, we should calculate ranking by using words statistics like basic tf-idf...
        var score = $u.max($u.map(fileMap[file], function(w){return scoreMap[file][w]}));
        results.push([docnames[file], titles[file], '', null, score, filenames[file]]);
      }
    }
    return results;
  },

  /**
   * helper function to return a node containing the
   * search summary for a given text. keywords is a list
   * of stemmed words, hlwords is the list of normal, unstemmed
   * words. the first one is used to find the occurrence, the
   * latter for highlighting it.
   */
  makeSearchSummary : function(text, keywords, hlwords) {
    var textLower = text.toLowerCase();
    var start = 0;
    $.each(keywords, function() {
      var i = textLower.indexOf(this.toLowerCase());
      if (i > -1)
        start = i;
    });
    start = Math.max(start - 120, 0);
    var excerpt = ((start > 0) ? '...' : '') +
      $.trim(text.substr(start, 240)) +
      ((start + 240 - text.length) ? '...' : '');
    var rv = $('<div class="context"></div>').text(excerpt);
    $.each(hlwords, function() {
      rv = rv.highlightText(this, 'highlighted');
    });
    return rv;
  }
};

$(document).ready(function() {
  Search.init();
});