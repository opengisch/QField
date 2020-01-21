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
var JSX={};(function(j){function l(b,e){var a=function(){};a.prototype=e.prototype;var c=new a;for(var d in b){b[d].prototype=c}}function M(c,b){for(var a in b.prototype)if(b.prototype.hasOwnProperty(a))c.prototype[a]=b.prototype[a]}function f(a,b,d){function c(a,b,c){delete a[b];a[b]=c;return c}Object.defineProperty(a,b,{get:function(){return c(a,b,d())},set:function(d){c(a,b,d)},enumerable:true,configurable:true})}function N(a,b,c){return a[b]=a[b]/c|0}var s=parseInt;var C=parseFloat;function O(a){return a!==a}var A=isFinite;var z=encodeURIComponent;var y=decodeURIComponent;var x=encodeURI;var v=decodeURI;var u=Object.prototype.toString;var E=Object.prototype.hasOwnProperty;function k(){}j.require=function(b){var a=q[b];return a!==undefined?a:null};j.profilerIsRunning=function(){return k.getResults!=null};j.getProfileResults=function(){return(k.getResults||function(){return{}})()};j.postProfileResults=function(a,b){if(k.postResults==null)throw new Error('profiler has not been turned on');return k.postResults(a,b)};j.resetProfileResults=function(){if(k.resetResults==null)throw new Error('profiler has not been turned on');return k.resetResults()};j.DEBUG=false;function t(){};l([t],Error);function b(a,b,c){this.F=a.length;this.M=a;this.N=b;this.H=c;this.G=null;this.S=null};function m(a,b,c,d,e){this.F=a.length;this.M=a;this.N=b;this.H=c;this.G=d;this.S=e};l([b,m],Object);function p(){};l([p],Object);function g(){var a;var b;var c;this.I={};a=this.E='';b=this._=0;c=this.A=a.length;this.B=0;this.C=b;this.D=c};l([g],p);function w(a,b){a.E=b.E;a._=b._;a.A=b.A;a.B=b.B;a.C=b.C;a.D=b.D};function n(b,d,c,e){var a;if(b._>=b.A){return false}a=b.E.charCodeAt(b._);if(a>e||a<c){return false}a-=c;if((d[a>>>3]&1<<(a&7))===0){return false}b._++;return true};g.prototype.L=function(c,b,d){var a;if(this._<=this.B){return false}a=this.E.charCodeAt(this._-1);if(a>d||a<b){return false}a-=b;if((c[a>>>3]&1<<(a&7))===0){return false}this._--;return true};function h(b,d,c,e){var a;if(b._<=b.B){return false}a=b.E.charCodeAt(b._-1);if(a>e||a<c){return false}a-=c;if((d[a>>>3]&1<<(a&7))===0){return false}b._--;return true};function o(a,d,c,e){var b;if(a._>=a.A){return false}b=a.E.charCodeAt(a._);if(b>e||b<c){a._++;return true}b-=c;if((d[b>>>3]&1<<(b&7))===0){a._++;return true}return false};function i(a,d,c,e){var b;if(a._<=a.B){return false}b=a.E.charCodeAt(a._-1);if(b>e||b<c){a._--;return true}b-=c;if((d[b>>>3]&1<<(b&7))===0){a._--;return true}return false};g.prototype.K=function(a,c){var b;if(this._-this.B<a){return false}if(this.E.slice((b=this._)-a,b)!==c){return false}this._-=a;return true};function c(a,b,d){var c;if(a._-a.B<b){return false}if(a.E.slice((c=a._)-b,c)!==d){return false}a._-=b;return true};g.prototype.Q=function(l,o){var b;var d;var e;var m;var f;var j;var k;var h;var g;var c;var a;var i;var n;b=0;d=o;e=this._;m=this.B;f=0;j=0;k=false;while(true){h=b+(d-b>>1);g=0;c=f<j?f:j;a=l[h];for(i=a.F-1-c;i>=0;i--){if(e-c===m){g=-1;break}g=this.E.charCodeAt(e-1-c)-a.M.charCodeAt(i);if(g!==0){break}c++}if(g<0){d=h;j=c}else{b=h;f=c}if(d-b<=1){if(b>0){break}if(d===b){break}if(k){break}k=true}}while(true){a=l[b];if(f>=a.F){this._=e-a.F|0;if(a.G==null){return a.H}n=a.G(this);this._=e-a.F|0;if(n){return a.H}}b=a.N;if(b<0){return 0}}return-1};function e(d,m,p){var b;var g;var e;var n;var f;var k;var l;var i;var h;var c;var a;var j;var o;b=0;g=p;e=d._;n=d.B;f=0;k=0;l=false;while(true){i=b+(g-b>>1);h=0;c=f<k?f:k;a=m[i];for(j=a.F-1-c;j>=0;j--){if(e-c===n){h=-1;break}h=d.E.charCodeAt(e-1-c)-a.M.charCodeAt(j);if(h!==0){break}c++}if(h<0){g=i;k=c}else{b=i;f=c}if(g-b<=1){if(b>0){break}if(g===b){break}if(l){break}l=true}}while(true){a=m[b];if(f>=a.F){d._=e-a.F|0;if(a.G==null){return a.H}o=a.G(d);d._=e-a.F|0;if(o){return a.H}}b=a.N;if(b<0){return 0}}return-1};function D(a,b,d,e){var c;c=e.length-(d-b);a.E=a.E.slice(0,b)+e+a.E.slice(d);a.A+=c|0;if(a._>=d){a._+=c|0}else if(a._>b){a._=b}return c|0};function d(a,f){var b;var c;var d;var e;b=false;if((c=a.C)<0||c>(d=a.D)||d>(e=a.A)||e>a.E.length?false:true){D(a,a.C,a.D,f);b=true}return b};function r(a,f){var b;var c;var d;var e;b='';if((c=a.C)<0||c>(d=a.D)||d>(e=a.A)||e>a.E.length?false:true){b=a.E.slice(a.C,a.D)}return b};g.prototype.J=function(){return false};g.prototype.e=function(b){var a;var c;var d;var e;a=this.I['.'+b];if(a==null){c=this.E=b;d=this._=0;e=this.A=c.length;this.B=0;this.C=d;this.D=e;this.J();a=this.E;this.I['.'+b]=a}return a};g.prototype.stemWord=g.prototype.e;g.prototype.f=function(e){var d;var b;var c;var a;var f;var g;var h;d=[];for(b=0;b<e.length;b++){c=e[b];a=this.I['.'+c];if(a==null){f=this.E=c;g=this._=0;h=this.A=f.length;this.B=0;this.C=g;this.D=h;this.J();a=this.E;this.I['.'+c]=a}d.push(a)}return d};g.prototype.stemWords=g.prototype.f;function a(){g.call(this);this.B_ending_removed=false;this.S_x='';this.I_p2=0;this.I_p1=0};l([a],g);a.prototype.O=function(a){this.B_ending_removed=a.B_ending_removed;this.S_x=a.S_x;this.I_p2=a.I_p2;this.I_p1=a.I_p1;w(this,a)};a.prototype.copy_from=a.prototype.O;a.prototype.Y=function(){var b;var c;var d;var e;var f;var g;var h;var i;var j;this.I_p1=i=this.A;this.I_p2=i;a:while(true){b=this._;d=true;b:while(d===true){d=false;if(!n(this,a.g_V1,97,246)){break b}this._=b;break a}h=this._=b;if(h>=this.A){return false}this._++}a:while(true){e=true;b:while(e===true){e=false;if(!o(this,a.g_V1,97,246)){break b}break a}if(this._>=this.A){return false}this._++}this.I_p1=this._;a:while(true){c=this._;f=true;b:while(f===true){f=false;if(!n(this,a.g_V1,97,246)){break b}this._=c;break a}j=this._=c;if(j>=this.A){return false}this._++}a:while(true){g=true;b:while(g===true){g=false;if(!o(this,a.g_V1,97,246)){break b}break a}if(this._>=this.A){return false}this._++}this.I_p2=this._;return true};a.prototype.r_mark_regions=a.prototype.Y;function H(b){var d;var e;var f;var c;var g;var h;var j;var k;var i;b.I_p1=k=b.A;b.I_p2=k;a:while(true){d=b._;f=true;b:while(f===true){f=false;if(!n(b,a.g_V1,97,246)){break b}b._=d;break a}j=b._=d;if(j>=b.A){return false}b._++}a:while(true){c=true;b:while(c===true){c=false;if(!o(b,a.g_V1,97,246)){break b}break a}if(b._>=b.A){return false}b._++}b.I_p1=b._;a:while(true){e=b._;g=true;b:while(g===true){g=false;if(!n(b,a.g_V1,97,246)){break b}b._=e;break a}i=b._=e;if(i>=b.A){return false}b._++}a:while(true){h=true;b:while(h===true){h=false;if(!o(b,a.g_V1,97,246)){break b}break a}if(b._>=b.A){return false}b._++}b.I_p2=b._;return true};a.prototype.U=function(){return!(this.I_p2<=this._)?false:true};a.prototype.r_R2=a.prototype.U;a.prototype.a=function(){var b;var f;var c;var g;var i;var j;f=this.A-(g=this._);if(g<this.I_p1){return false}i=this._=this.I_p1;c=this.B;this.B=i;j=this._=this.A-f;this.D=j;b=e(this,a.a_0,10);if(b===0){this.B=c;return false}this.C=this._;this.B=c;switch(b){case 0:return false;case 1:if(!h(this,a.g_particle_end,97,246)){return false}break;case 2:if(!(!(this.I_p2<=this._)?false:true)){return false}break}return!d(this,'')?false:true};a.prototype.r_particle_etc=a.prototype.a;function I(b){var c;var g;var f;var i;var j;var k;g=b.A-(i=b._);if(i<b.I_p1){return false}j=b._=b.I_p1;f=b.B;b.B=j;k=b._=b.A-g;b.D=k;c=e(b,a.a_0,10);if(c===0){b.B=f;return false}b.C=b._;b.B=f;switch(c){case 0:return false;case 1:if(!h(b,a.g_particle_end,97,246)){return false}break;case 2:if(!(!(b.I_p2<=b._)?false:true)){return false}break}return!d(b,'')?false:true};a.prototype.b=function(){var b;var h;var f;var i;var g;var j;var k;var l;h=this.A-(j=this._);if(j<this.I_p1){return false}k=this._=this.I_p1;f=this.B;this.B=k;l=this._=this.A-h;this.D=l;b=e(this,a.a_4,9);if(b===0){this.B=f;return false}this.C=this._;this.B=f;switch(b){case 0:return false;case 1:i=this.A-this._;g=true;a:while(g===true){g=false;if(!c(this,1,'k')){break a}return false}this._=this.A-i;if(!d(this,'')){return false}break;case 2:if(!d(this,'')){return false}this.D=this._;if(!c(this,3,'kse')){return false}this.C=this._;if(!d(this,'ksi')){return false}break;case 3:if(!d(this,'')){return false}break;case 4:if(e(this,a.a_1,6)===0){return false}if(!d(this,'')){return false}break;case 5:if(e(this,a.a_2,6)===0){return false}if(!d(this,'')){return false}break;case 6:if(e(this,a.a_3,2)===0){return false}if(!d(this,'')){return false}break}return true};a.prototype.r_possessive=a.prototype.b;function J(b){var f;var i;var g;var j;var h;var k;var l;var m;i=b.A-(k=b._);if(k<b.I_p1){return false}l=b._=b.I_p1;g=b.B;b.B=l;m=b._=b.A-i;b.D=m;f=e(b,a.a_4,9);if(f===0){b.B=g;return false}b.C=b._;b.B=g;switch(f){case 0:return false;case 1:j=b.A-b._;h=true;a:while(h===true){h=false;if(!c(b,1,'k')){break a}return false}b._=b.A-j;if(!d(b,'')){return false}break;case 2:if(!d(b,'')){return false}b.D=b._;if(!c(b,3,'kse')){return false}b.C=b._;if(!d(b,'ksi')){return false}break;case 3:if(!d(b,'')){return false}break;case 4:if(e(b,a.a_1,6)===0){return false}if(!d(b,'')){return false}break;case 5:if(e(b,a.a_2,6)===0){return false}if(!d(b,'')){return false}break;case 6:if(e(b,a.a_3,2)===0){return false}if(!d(b,'')){return false}break}return true};a.prototype.T=function(){return e(this,a.a_5,7)===0?false:true};a.prototype.r_LONG=a.prototype.T;a.prototype.V=function(){return!c(this,1,'i')?false:!h(this,a.g_V2,97,246)?false:true};a.prototype.r_VI=a.prototype.V;a.prototype.W=function(){var j;var o;var f;var g;var p;var m;var b;var k;var l;var q;var r;var s;var n;o=this.A-(q=this._);if(q<this.I_p1){return false}r=this._=this.I_p1;f=this.B;this.B=r;s=this._=this.A-o;this.D=s;j=e(this,a.a_6,30);if(j===0){this.B=f;return false}this.C=this._;this.B=f;switch(j){case 0:return false;case 1:if(!c(this,1,'a')){return false}break;case 2:if(!c(this,1,'e')){return false}break;case 3:if(!c(this,1,'i')){return false}break;case 4:if(!c(this,1,'o')){return false}break;case 5:if(!c(this,1,'ä')){return false}break;case 6:if(!c(this,1,'ö')){return false}break;case 7:g=this.A-this._;b=true;a:while(b===true){b=false;p=this.A-this._;k=true;b:while(k===true){k=false;m=this.A-this._;l=true;c:while(l===true){l=false;if(!(e(this,a.a_5,7)===0?false:true)){break c}break b}this._=this.A-m;if(!c(this,2,'ie')){this._=this.A-g;break a}}n=this._=this.A-p;if(n<=this.B){this._=this.A-g;break a}this._--;this.C=this._}break;case 8:if(!h(this,a.g_V1,97,246)){return false}if(!i(this,a.g_V1,97,246)){return false}break;case 9:if(!c(this,1,'e')){return false}break}if(!d(this,'')){return false}this.B_ending_removed=true;return true};a.prototype.r_case_ending=a.prototype.W;function K(b){var f;var o;var g;var j;var p;var n;var k;var l;var m;var r;var s;var t;var q;o=b.A-(r=b._);if(r<b.I_p1){return false}s=b._=b.I_p1;g=b.B;b.B=s;t=b._=b.A-o;b.D=t;f=e(b,a.a_6,30);if(f===0){b.B=g;return false}b.C=b._;b.B=g;switch(f){case 0:return false;case 1:if(!c(b,1,'a')){return false}break;case 2:if(!c(b,1,'e')){return false}break;case 3:if(!c(b,1,'i')){return false}break;case 4:if(!c(b,1,'o')){return false}break;case 5:if(!c(b,1,'ä')){return false}break;case 6:if(!c(b,1,'ö')){return false}break;case 7:j=b.A-b._;k=true;a:while(k===true){k=false;p=b.A-b._;l=true;b:while(l===true){l=false;n=b.A-b._;m=true;c:while(m===true){m=false;if(!(e(b,a.a_5,7)===0?false:true)){break c}break b}b._=b.A-n;if(!c(b,2,'ie')){b._=b.A-j;break a}}q=b._=b.A-p;if(q<=b.B){b._=b.A-j;break a}b._--;b.C=b._}break;case 8:if(!h(b,a.g_V1,97,246)){return false}if(!i(b,a.g_V1,97,246)){return false}break;case 9:if(!c(b,1,'e')){return false}break}if(!d(b,'')){return false}b.B_ending_removed=true;return true};a.prototype.Z=function(){var b;var h;var f;var i;var g;var j;var k;var l;h=this.A-(j=this._);if(j<this.I_p2){return false}k=this._=this.I_p2;f=this.B;this.B=k;l=this._=this.A-h;this.D=l;b=e(this,a.a_7,14);if(b===0){this.B=f;return false}this.C=this._;this.B=f;switch(b){case 0:return false;case 1:i=this.A-this._;g=true;a:while(g===true){g=false;if(!c(this,2,'po')){break a}return false}this._=this.A-i;break}return!d(this,'')?false:true};a.prototype.r_other_endings=a.prototype.Z;function L(b){var f;var i;var g;var j;var h;var k;var l;var m;i=b.A-(k=b._);if(k<b.I_p2){return false}l=b._=b.I_p2;g=b.B;b.B=l;m=b._=b.A-i;b.D=m;f=e(b,a.a_7,14);if(f===0){b.B=g;return false}b.C=b._;b.B=g;switch(f){case 0:return false;case 1:j=b.A-b._;h=true;a:while(h===true){h=false;if(!c(b,2,'po')){break a}return false}b._=b.A-j;break}return!d(b,'')?false:true};a.prototype.X=function(){var c;var b;var f;var g;var h;c=this.A-(f=this._);if(f<this.I_p1){return false}g=this._=this.I_p1;b=this.B;this.B=g;h=this._=this.A-c;this.D=h;if(e(this,a.a_8,2)===0){this.B=b;return false}this.C=this._;this.B=b;return!d(this,'')?false:true};a.prototype.r_i_plural=a.prototype.X;function G(b){var f;var c;var g;var h;var i;f=b.A-(g=b._);if(g<b.I_p1){return false}h=b._=b.I_p1;c=b.B;b.B=h;i=b._=b.A-f;b.D=i;if(e(b,a.a_8,2)===0){b.B=c;return false}b.C=b._;b.B=c;return!d(b,'')?false:true};a.prototype.c=function(){var i;var l;var b;var j;var k;var g;var m;var f;var o;var p;var q;var r;var s;var t;var n;l=this.A-(o=this._);if(o<this.I_p1){return false}p=this._=this.I_p1;b=this.B;this.B=p;q=this._=this.A-l;this.D=q;if(!c(this,1,'t')){this.B=b;return false}this.C=r=this._;j=this.A-r;if(!h(this,a.g_V1,97,246)){this.B=b;return false}this._=this.A-j;if(!d(this,'')){return false}this.B=b;k=this.A-(s=this._);if(s<this.I_p2){return false}t=this._=this.I_p2;g=this.B;this.B=t;n=this._=this.A-k;this.D=n;i=e(this,a.a_9,2);if(i===0){this.B=g;return false}this.C=this._;this.B=g;switch(i){case 0:return false;case 1:m=this.A-this._;f=true;a:while(f===true){f=false;if(!c(this,2,'po')){break a}return false}this._=this.A-m;break}return!d(this,'')?false:true};a.prototype.r_t_plural=a.prototype.c;function F(b){var g;var m;var f;var o;var l;var i;var k;var j;var p;var q;var r;var s;var t;var u;var n;m=b.A-(p=b._);if(p<b.I_p1){return false}q=b._=b.I_p1;f=b.B;b.B=q;r=b._=b.A-m;b.D=r;if(!c(b,1,'t')){b.B=f;return false}b.C=s=b._;o=b.A-s;if(!h(b,a.g_V1,97,246)){b.B=f;return false}b._=b.A-o;if(!d(b,'')){return false}b.B=f;l=b.A-(t=b._);if(t<b.I_p2){return false}u=b._=b.I_p2;i=b.B;b.B=u;n=b._=b.A-l;b.D=n;g=e(b,a.a_9,2);if(g===0){b.B=i;return false}b.C=b._;b.B=i;switch(g){case 0:return false;case 1:k=b.A-b._;j=true;a:while(j===true){j=false;if(!c(b,2,'po')){break a}return false}b._=b.A-k;break}return!d(b,'')?false:true};a.prototype.d=function(){var x;var q;var s;var t;var u;var v;var w;var y;var f;var g;var j;var k;var l;var m;var n;var b;var o;var z;var p;var B;var C;var D;var E;var F;var G;var H;var I;var J;var K;var L;var A;x=this.A-(z=this._);if(z<this.I_p1){return false}B=this._=this.I_p1;q=this.B;this.B=B;D=this._=(C=this.A)-x;s=C-D;g=true;a:while(g===true){g=false;t=this.A-this._;if(!(e(this,a.a_5,7)===0?false:true)){break a}p=this._=this.A-t;this.D=p;if(p<=this.B){break a}this._--;this.C=this._;if(!d(this,'')){return false}}F=this._=(E=this.A)-s;u=E-F;j=true;a:while(j===true){j=false;this.D=this._;if(!h(this,a.g_AEI,97,228)){break a}this.C=this._;if(!i(this,a.g_V1,97,246)){break a}if(!d(this,'')){return false}}H=this._=(G=this.A)-u;v=G-H;k=true;a:while(k===true){k=false;this.D=this._;if(!c(this,1,'j')){break a}this.C=this._;l=true;b:while(l===true){l=false;w=this.A-this._;m=true;c:while(m===true){m=false;if(!c(this,1,'o')){break c}break b}this._=this.A-w;if(!c(this,1,'u')){break a}}if(!d(this,'')){return false}}J=this._=(I=this.A)-v;y=I-J;n=true;a:while(n===true){n=false;this.D=this._;if(!c(this,1,'o')){break a}this.C=this._;if(!c(this,1,'j')){break a}if(!d(this,'')){return false}}this._=this.A-y;this.B=q;a:while(true){f=this.A-this._;b=true;b:while(b===true){b=false;if(!i(this,a.g_V1,97,246)){break b}this._=this.A-f;break a}K=this._=this.A-f;if(K<=this.B){return false}this._--}this.D=L=this._;if(L<=this.B){return false}this._--;this.C=this._;A=this.S_x=r(this,this.S_x);return A===''?false:!(o=this.S_x,c(this,o.length,o))?false:!d(this,'')?false:true};a.prototype.r_tidy=a.prototype.d;function B(b){var s;var t;var u;var v;var w;var x;var y;var z;var l;var g;var j;var k;var f;var m;var n;var o;var p;var A;var q;var C;var D;var E;var F;var G;var H;var I;var J;var K;var L;var M;var B;s=b.A-(A=b._);if(A<b.I_p1){return false}C=b._=b.I_p1;t=b.B;b.B=C;E=b._=(D=b.A)-s;u=D-E;g=true;a:while(g===true){g=false;v=b.A-b._;if(!(e(b,a.a_5,7)===0?false:true)){break a}q=b._=b.A-v;b.D=q;if(q<=b.B){break a}b._--;b.C=b._;if(!d(b,'')){return false}}G=b._=(F=b.A)-u;w=F-G;j=true;a:while(j===true){j=false;b.D=b._;if(!h(b,a.g_AEI,97,228)){break a}b.C=b._;if(!i(b,a.g_V1,97,246)){break a}if(!d(b,'')){return false}}I=b._=(H=b.A)-w;x=H-I;k=true;a:while(k===true){k=false;b.D=b._;if(!c(b,1,'j')){break a}b.C=b._;f=true;b:while(f===true){f=false;y=b.A-b._;m=true;c:while(m===true){m=false;if(!c(b,1,'o')){break c}break b}b._=b.A-y;if(!c(b,1,'u')){break a}}if(!d(b,'')){return false}}K=b._=(J=b.A)-x;z=J-K;n=true;a:while(n===true){n=false;b.D=b._;if(!c(b,1,'o')){break a}b.C=b._;if(!c(b,1,'j')){break a}if(!d(b,'')){return false}}b._=b.A-z;b.B=t;a:while(true){l=b.A-b._;o=true;b:while(o===true){o=false;if(!i(b,a.g_V1,97,246)){break b}b._=b.A-l;break a}L=b._=b.A-l;if(L<=b.B){return false}b._--}b.D=M=b._;if(M<=b.B){return false}b._--;b.C=b._;B=b.S_x=r(b,b.S_x);return B===''?false:!(p=b.S_x,c(b,p.length,p))?false:!d(b,'')?false:true};a.prototype.J=function(){var p;var k;var l;var m;var n;var o;var q;var r;var b;var c;var d;var e;var f;var g;var a;var h;var i;var j;var t;var u;var v;var w;var x;var y;var z;var A;var C;var D;var s;p=this._;b=true;a:while(b===true){b=false;if(!H(this)){break a}}t=this._=p;this.B_ending_removed=false;this.B=t;v=this._=u=this.A;k=u-v;c=true;a:while(c===true){c=false;if(!I(this)){break a}}x=this._=(w=this.A)-k;l=w-x;d=true;a:while(d===true){d=false;if(!J(this)){break a}}z=this._=(y=this.A)-l;m=y-z;e=true;a:while(e===true){e=false;if(!K(this)){break a}}C=this._=(A=this.A)-m;n=A-C;f=true;a:while(f===true){f=false;if(!L(this)){break a}}this._=this.A-n;g=true;a:while(g===true){g=false;o=this.A-this._;a=true;b:while(a===true){a=false;if(!this.B_ending_removed){break b}q=this.A-this._;h=true;c:while(h===true){h=false;if(!G(this)){break c}}this._=this.A-q;break a}s=this._=(D=this.A)-o;r=D-s;i=true;b:while(i===true){i=false;if(!F(this)){break b}}this._=this.A-r}j=true;a:while(j===true){j=false;if(!B(this)){break a}}this._=this.B;return true};a.prototype.stem=a.prototype.J;a.prototype.P=function(b){return b instanceof a};a.prototype.equals=a.prototype.P;a.prototype.R=function(){var c;var a;var b;var d;c='FinnishStemmer';a=0;for(b=0;b<c.length;b++){d=c.charCodeAt(b);a=(a<<5)-a+d;a=a&a}return a|0};a.prototype.hashCode=a.prototype.R;a.serialVersionUID=1;f(a,'methodObject',function(){return new a});f(a,'a_0',function(){return[new b('pa',-1,1),new b('sti',-1,2),new b('kaan',-1,1),new b('han',-1,1),new b('kin',-1,1),new b('hän',-1,1),new b('kään',-1,1),new b('ko',-1,1),new b('pä',-1,1),new b('kö',-1,1)]});f(a,'a_1',function(){return[new b('lla',-1,-1),new b('na',-1,-1),new b('ssa',-1,-1),new b('ta',-1,-1),new b('lta',3,-1),new b('sta',3,-1)]});f(a,'a_2',function(){return[new b('llä',-1,-1),new b('nä',-1,-1),new b('ssä',-1,-1),new b('tä',-1,-1),new b('ltä',3,-1),new b('stä',3,-1)]});f(a,'a_3',function(){return[new b('lle',-1,-1),new b('ine',-1,-1)]});f(a,'a_4',function(){return[new b('nsa',-1,3),new b('mme',-1,3),new b('nne',-1,3),new b('ni',-1,2),new b('si',-1,1),new b('an',-1,4),new b('en',-1,6),new b('än',-1,5),new b('nsä',-1,3)]});f(a,'a_5',function(){return[new b('aa',-1,-1),new b('ee',-1,-1),new b('ii',-1,-1),new b('oo',-1,-1),new b('uu',-1,-1),new b('ää',-1,-1),new b('öö',-1,-1)]});f(a,'a_6',function(){return[new b('a',-1,8),new b('lla',0,-1),new b('na',0,-1),new b('ssa',0,-1),new b('ta',0,-1),new b('lta',4,-1),new b('sta',4,-1),new b('tta',4,9),new b('lle',-1,-1),new b('ine',-1,-1),new b('ksi',-1,-1),new b('n',-1,7),new b('han',11,1),new m('den',11,-1,function(c){var b;b=c;return!b.K(1,'i')?false:!b.L(a.g_V2,97,246)?false:true},a.methodObject),new m('seen',11,-1,function(c){var b;b=c;return b.Q(a.a_5,7)===0?false:true},a.methodObject),new b('hen',11,2),new m('tten',11,-1,function(c){var b;b=c;return!b.K(1,'i')?false:!b.L(a.g_V2,97,246)?false:true},a.methodObject),new b('hin',11,3),new m('siin',11,-1,function(c){var b;b=c;return!b.K(1,'i')?false:!b.L(a.g_V2,97,246)?false:true},a.methodObject),new b('hon',11,4),new b('hän',11,5),new b('hön',11,6),new b('ä',-1,8),new b('llä',22,-1),new b('nä',22,-1),new b('ssä',22,-1),new b('tä',22,-1),new b('ltä',26,-1),new b('stä',26,-1),new b('ttä',26,9)]});f(a,'a_7',function(){return[new b('eja',-1,-1),new b('mma',-1,1),new b('imma',1,-1),new b('mpa',-1,1),new b('impa',3,-1),new b('mmi',-1,1),new b('immi',5,-1),new b('mpi',-1,1),new b('impi',7,-1),new b('ejä',-1,-1),new b('mmä',-1,1),new b('immä',10,-1),new b('mpä',-1,1),new b('impä',12,-1)]});f(a,'a_8',function(){return[new b('i',-1,-1),new b('j',-1,-1)]});f(a,'a_9',function(){return[new b('mma',-1,1),new b('imma',0,-1)]});f(a,'g_AEI',function(){return[17,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8]});f(a,'g_V1',function(){return[17,65,16,1,0,0,0,0,0,0,0,0,0,0,0,0,8,0,32]});f(a,'g_V2',function(){return[17,65,16,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,32]});f(a,'g_particle_end',function(){return[17,97,24,1,0,0,0,0,0,0,0,0,0,0,0,0,8,0,32]});var q={'src/stemmer.jsx':{Stemmer:p},'src/finnish-stemmer.jsx':{FinnishStemmer:a}}}(JSX))
var Stemmer = JSX.require("src/finnish-stemmer.jsx").FinnishStemmer;



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
    var stopwords = ["ei","eiv\u00e4t","emme","en","et","ette","ett\u00e4","he","heid\u00e4n","heid\u00e4t","heihin","heille","heill\u00e4","heilt\u00e4","heiss\u00e4","heist\u00e4","heit\u00e4","h\u00e4n","h\u00e4neen","h\u00e4nelle","h\u00e4nell\u00e4","h\u00e4nelt\u00e4","h\u00e4nen","h\u00e4ness\u00e4","h\u00e4nest\u00e4","h\u00e4net","h\u00e4nt\u00e4","itse","ja","johon","joiden","joihin","joiksi","joilla","joille","joilta","joina","joissa","joista","joita","joka","joksi","jolla","jolle","jolta","jona","jonka","jos","jossa","josta","jota","jotka","kanssa","keiden","keihin","keiksi","keille","keill\u00e4","keilt\u00e4","kein\u00e4","keiss\u00e4","keist\u00e4","keit\u00e4","keneen","keneksi","kenelle","kenell\u00e4","kenelt\u00e4","kenen","kenen\u00e4","keness\u00e4","kenest\u00e4","kenet","ketk\u00e4","ket\u00e4","koska","kuin","kuka","kun","me","meid\u00e4n","meid\u00e4t","meihin","meille","meill\u00e4","meilt\u00e4","meiss\u00e4","meist\u00e4","meit\u00e4","mihin","miksi","mik\u00e4","mille","mill\u00e4","milt\u00e4","mink\u00e4","minua","minulla","minulle","minulta","minun","minussa","minusta","minut","minuun","min\u00e4","miss\u00e4","mist\u00e4","mitk\u00e4","mit\u00e4","mukaan","mutta","ne","niiden","niihin","niiksi","niille","niill\u00e4","niilt\u00e4","niin","niin\u00e4","niiss\u00e4","niist\u00e4","niit\u00e4","noiden","noihin","noiksi","noilla","noille","noilta","noin","noina","noissa","noista","noita","nuo","nyt","n\u00e4iden","n\u00e4ihin","n\u00e4iksi","n\u00e4ille","n\u00e4ill\u00e4","n\u00e4ilt\u00e4","n\u00e4in\u00e4","n\u00e4iss\u00e4","n\u00e4ist\u00e4","n\u00e4it\u00e4","n\u00e4m\u00e4","ole","olemme","olen","olet","olette","oli","olimme","olin","olisi","olisimme","olisin","olisit","olisitte","olisivat","olit","olitte","olivat","olla","olleet","ollut","on","ovat","poikki","se","sek\u00e4","sen","siihen","siin\u00e4","siit\u00e4","siksi","sille","sill\u00e4","silt\u00e4","sinua","sinulla","sinulle","sinulta","sinun","sinussa","sinusta","sinut","sinuun","sin\u00e4","sit\u00e4","tai","te","teid\u00e4n","teid\u00e4t","teihin","teille","teill\u00e4","teilt\u00e4","teiss\u00e4","teist\u00e4","teit\u00e4","tuo","tuohon","tuoksi","tuolla","tuolle","tuolta","tuon","tuona","tuossa","tuosta","tuota","t\u00e4h\u00e4n","t\u00e4ksi","t\u00e4lle","t\u00e4ll\u00e4","t\u00e4lt\u00e4","t\u00e4m\u00e4","t\u00e4m\u00e4n","t\u00e4n\u00e4","t\u00e4ss\u00e4","t\u00e4st\u00e4","t\u00e4t\u00e4","vaan","vai","vaikka","yli"];

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