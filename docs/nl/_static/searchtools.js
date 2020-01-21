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
var JSX={};(function(m){function n(b,e){var a=function(){};a.prototype=e.prototype;var c=new a;for(var d in b){b[d].prototype=c}}function L(c,b){for(var a in b.prototype)if(b.prototype.hasOwnProperty(a))c.prototype[a]=b.prototype[a]}function e(a,b,d){function c(a,b,c){delete a[b];a[b]=c;return c}Object.defineProperty(a,b,{get:function(){return c(a,b,d())},set:function(d){c(a,b,d)},enumerable:true,configurable:true})}function K(a,b,c){return a[b]=a[b]/c|0}var I=parseInt;var E=parseFloat;function M(a){return a!==a}var B=isFinite;var A=encodeURIComponent;var z=decodeURIComponent;var y=encodeURI;var x=decodeURI;var w=Object.prototype.toString;var C=Object.prototype.hasOwnProperty;function l(){}m.require=function(b){var a=t[b];return a!==undefined?a:null};m.profilerIsRunning=function(){return l.getResults!=null};m.getProfileResults=function(){return(l.getResults||function(){return{}})()};m.postProfileResults=function(a,b){if(l.postResults==null)throw new Error('profiler has not been turned on');return l.postResults(a,b)};m.resetProfileResults=function(){if(l.resetResults==null)throw new Error('profiler has not been turned on');return l.resetResults()};m.DEBUG=false;function v(){};n([v],Error);function c(a,b,c){this.F=a.length;this.K=a;this.L=b;this.I=c;this.H=null;this.P=null};n([c],Object);function s(){};n([s],Object);function g(){var a;var b;var c;this.G={};a=this.D='';b=this._=0;c=this.A=a.length;this.E=0;this.C=b;this.B=c};n([g],s);function D(a,b){a.D=b.D;a._=b._;a.A=b.A;a.E=b.E;a.C=b.C;a.B=b.B};function i(b,d,c,e){var a;if(b._>=b.A){return false}a=b.D.charCodeAt(b._);if(a>e||a<c){return false}a-=c;if((d[a>>>3]&1<<(a&7))===0){return false}b._++;return true};function r(a,d,c,e){var b;if(a._>=a.A){return false}b=a.D.charCodeAt(a._);if(b>e||b<c){a._++;return true}b-=c;if((d[b>>>3]&1<<(b&7))===0){a._++;return true}return false};function f(a,d,c,e){var b;if(a._<=a.E){return false}b=a.D.charCodeAt(a._-1);if(b>e||b<c){a._--;return true}b-=c;if((d[b>>>3]&1<<(b&7))===0){a._--;return true}return false};function k(a,b,d){var c;if(a.A-a._<b){return false}if(a.D.slice(c=a._,c+b)!==d){return false}a._+=b;return true};function d(a,b,d){var c;if(a._-a.E<b){return false}if(a.D.slice((c=a._)-b,c)!==d){return false}a._-=b;return true};function q(f,m,p){var b;var d;var e;var n;var g;var k;var l;var i;var h;var c;var a;var j;var o;b=0;d=p;e=f._;n=f.A;g=0;k=0;l=false;while(true){i=b+(d-b>>>1);h=0;c=g<k?g:k;a=m[i];for(j=c;j<a.F;j++){if(e+c===n){h=-1;break}h=f.D.charCodeAt(e+c)-a.K.charCodeAt(j);if(h!==0){break}c++}if(h<0){d=i;k=c}else{b=i;g=c}if(d-b<=1){if(b>0){break}if(d===b){break}if(l){break}l=true}}while(true){a=m[b];if(g>=a.F){f._=e+a.F|0;if(a.H==null){return a.I}o=a.H(a.P);f._=e+a.F|0;if(o){return a.I}}b=a.L;if(b<0){return 0}}return-1};function h(d,m,p){var b;var g;var e;var n;var f;var k;var l;var i;var h;var c;var a;var j;var o;b=0;g=p;e=d._;n=d.E;f=0;k=0;l=false;while(true){i=b+(g-b>>1);h=0;c=f<k?f:k;a=m[i];for(j=a.F-1-c;j>=0;j--){if(e-c===n){h=-1;break}h=d.D.charCodeAt(e-1-c)-a.K.charCodeAt(j);if(h!==0){break}c++}if(h<0){g=i;k=c}else{b=i;f=c}if(g-b<=1){if(b>0){break}if(g===b){break}if(l){break}l=true}}while(true){a=m[b];if(f>=a.F){d._=e-a.F|0;if(a.H==null){return a.I}o=a.H(d);d._=e-a.F|0;if(o){return a.I}}b=a.L;if(b<0){return 0}}return-1};function u(a,b,d,e){var c;c=e.length-(d-b);a.D=a.D.slice(0,b)+e+a.D.slice(d);a.A+=c|0;if(a._>=d){a._+=c|0}else if(a._>b){a._=b}return c|0};function b(a,f){var b;var c;var d;var e;b=false;if((c=a.C)<0||c>(d=a.B)||d>(e=a.A)||e>a.D.length?false:true){u(a,a.C,a.B,f);b=true}return b};g.prototype.J=function(){return false};g.prototype.Z=function(b){var a;var c;var d;var e;a=this.G['.'+b];if(a==null){c=this.D=b;d=this._=0;e=this.A=c.length;this.E=0;this.C=d;this.B=e;this.J();a=this.D;this.G['.'+b]=a}return a};g.prototype.stemWord=g.prototype.Z;g.prototype.a=function(e){var d;var b;var c;var a;var f;var g;var h;d=[];for(b=0;b<e.length;b++){c=e[b];a=this.G['.'+c];if(a==null){f=this.D=c;g=this._=0;h=this.A=f.length;this.E=0;this.C=g;this.B=h;this.J();a=this.D;this.G['.'+c]=a}d.push(a)}return d};g.prototype.stemWords=g.prototype.a;function a(){g.call(this);this.I_p2=0;this.I_p1=0;this.B_e_found=false};n([a],g);a.prototype.M=function(a){this.I_p2=a.I_p2;this.I_p1=a.I_p1;this.B_e_found=a.B_e_found;D(this,a)};a.prototype.copy_from=a.prototype.M;a.prototype.W=function(){var e;var m;var n;var o;var p;var d;var s;var c;var f;var g;var h;var j;var l;var t;var r;m=this._;b:while(true){n=this._;c=true;a:while(c===true){c=false;this.C=this._;e=q(this,a.a_0,11);if(e===0){break a}this.B=this._;switch(e){case 0:break a;case 1:if(!b(this,'a')){return false}break;case 2:if(!b(this,'e')){return false}break;case 3:if(!b(this,'i')){return false}break;case 4:if(!b(this,'o')){return false}break;case 5:if(!b(this,'u')){return false}break;case 6:if(this._>=this.A){break a}this._++;break}continue b}this._=n;break b}t=this._=m;o=t;f=true;a:while(f===true){f=false;this.C=this._;if(!k(this,1,'y')){this._=o;break a}this.B=this._;if(!b(this,'Y')){return false}}a:while(true){p=this._;g=true;d:while(g===true){g=false;e:while(true){d=this._;h=true;b:while(h===true){h=false;if(!i(this,a.g_v,97,232)){break b}this.C=this._;j=true;f:while(j===true){j=false;s=this._;l=true;c:while(l===true){l=false;if(!k(this,1,'i')){break c}this.B=this._;if(!i(this,a.g_v,97,232)){break c}if(!b(this,'I')){return false}break f}this._=s;if(!k(this,1,'y')){break b}this.B=this._;if(!b(this,'Y')){return false}}this._=d;break e}r=this._=d;if(r>=this.A){break d}this._++}continue a}this._=p;break a}return true};a.prototype.r_prelude=a.prototype.W;function F(c){var d;var s;var t;var o;var p;var e;var n;var f;var g;var h;var j;var l;var m;var u;var r;s=c._;b:while(true){t=c._;f=true;a:while(f===true){f=false;c.C=c._;d=q(c,a.a_0,11);if(d===0){break a}c.B=c._;switch(d){case 0:break a;case 1:if(!b(c,'a')){return false}break;case 2:if(!b(c,'e')){return false}break;case 3:if(!b(c,'i')){return false}break;case 4:if(!b(c,'o')){return false}break;case 5:if(!b(c,'u')){return false}break;case 6:if(c._>=c.A){break a}c._++;break}continue b}c._=t;break b}u=c._=s;o=u;g=true;a:while(g===true){g=false;c.C=c._;if(!k(c,1,'y')){c._=o;break a}c.B=c._;if(!b(c,'Y')){return false}}a:while(true){p=c._;h=true;d:while(h===true){h=false;e:while(true){e=c._;j=true;b:while(j===true){j=false;if(!i(c,a.g_v,97,232)){break b}c.C=c._;l=true;f:while(l===true){l=false;n=c._;m=true;c:while(m===true){m=false;if(!k(c,1,'i')){break c}c.B=c._;if(!i(c,a.g_v,97,232)){break c}if(!b(c,'I')){return false}break f}c._=n;if(!k(c,1,'y')){break b}c.B=c._;if(!b(c,'Y')){return false}}c._=e;break e}r=c._=e;if(r>=c.A){break d}c._++}continue a}c._=p;break a}return true};a.prototype.U=function(){var b;var c;var d;var e;var f;var g;this.I_p1=g=this.A;this.I_p2=g;a:while(true){b=true;b:while(b===true){b=false;if(!i(this,a.g_v,97,232)){break b}break a}if(this._>=this.A){return false}this._++}a:while(true){c=true;b:while(c===true){c=false;if(!r(this,a.g_v,97,232)){break b}break a}if(this._>=this.A){return false}this._++}this.I_p1=this._;d=true;a:while(d===true){d=false;if(!(this.I_p1<3)){break a}this.I_p1=3}a:while(true){e=true;b:while(e===true){e=false;if(!i(this,a.g_v,97,232)){break b}break a}if(this._>=this.A){return false}this._++}a:while(true){f=true;b:while(f===true){f=false;if(!r(this,a.g_v,97,232)){break b}break a}if(this._>=this.A){return false}this._++}this.I_p2=this._;return true};a.prototype.r_mark_regions=a.prototype.U;function G(b){var c;var d;var e;var f;var g;var h;b.I_p1=h=b.A;b.I_p2=h;a:while(true){c=true;b:while(c===true){c=false;if(!i(b,a.g_v,97,232)){break b}break a}if(b._>=b.A){return false}b._++}a:while(true){d=true;b:while(d===true){d=false;if(!r(b,a.g_v,97,232)){break b}break a}if(b._>=b.A){return false}b._++}b.I_p1=b._;e=true;a:while(e===true){e=false;if(!(b.I_p1<3)){break a}b.I_p1=3}a:while(true){f=true;b:while(f===true){f=false;if(!i(b,a.g_v,97,232)){break b}break a}if(b._>=b.A){return false}b._++}a:while(true){g=true;b:while(g===true){g=false;if(!r(b,a.g_v,97,232)){break b}break a}if(b._>=b.A){return false}b._++}b.I_p2=b._;return true};a.prototype.V=function(){var c;var e;var d;b:while(true){e=this._;d=true;a:while(d===true){d=false;this.C=this._;c=q(this,a.a_1,3);if(c===0){break a}this.B=this._;switch(c){case 0:break a;case 1:if(!b(this,'y')){return false}break;case 2:if(!b(this,'i')){return false}break;case 3:if(this._>=this.A){break a}this._++;break}continue b}this._=e;break b}return true};a.prototype.r_postlude=a.prototype.V;function H(c){var d;var f;var e;b:while(true){f=c._;e=true;a:while(e===true){e=false;c.C=c._;d=q(c,a.a_1,3);if(d===0){break a}c.B=c._;switch(d){case 0:break a;case 1:if(!b(c,'y')){return false}break;case 2:if(!b(c,'i')){return false}break;case 3:if(c._>=c.A){break a}c._++;break}continue b}c._=f;break b}return true};a.prototype.Q=function(){return!(this.I_p1<=this._)?false:true};a.prototype.r_R1=a.prototype.Q;a.prototype.R=function(){return!(this.I_p2<=this._)?false:true};a.prototype.r_R2=a.prototype.R;a.prototype.Y=function(){var d;var c;d=this.A-this._;if(h(this,a.a_2,3)===0){return false}c=this._=this.A-d;this.B=c;if(c<=this.E){return false}this._--;this.C=this._;return!b(this,'')?false:true};a.prototype.r_undouble=a.prototype.Y;function j(c){var e;var d;e=c.A-c._;if(h(c,a.a_2,3)===0){return false}d=c._=c.A-e;c.B=d;if(d<=c.E){return false}c._--;c.C=c._;return!b(c,'')?false:true};a.prototype.S=function(){var c;var e;this.B_e_found=false;this.B=this._;if(!d(this,1,'e')){return false}this.C=e=this._;if(!(!(this.I_p1<=e)?false:true)){return false}c=this.A-this._;if(!f(this,a.g_v,97,232)){return false}this._=this.A-c;if(!b(this,'')){return false}this.B_e_found=true;return!j(this)?false:true};a.prototype.r_e_ending=a.prototype.S;function o(c){var e;var g;c.B_e_found=false;c.B=c._;if(!d(c,1,'e')){return false}c.C=g=c._;if(!(!(c.I_p1<=g)?false:true)){return false}e=c.A-c._;if(!f(c,a.g_v,97,232)){return false}c._=c.A-e;if(!b(c,'')){return false}c.B_e_found=true;return!j(c)?false:true};a.prototype.T=function(){var e;var g;var c;var h;var i;if(!(!(this.I_p1<=this._)?false:true)){return false}e=this.A-this._;if(!f(this,a.g_v,97,232)){return false}i=this._=(h=this.A)-e;g=h-i;c=true;a:while(c===true){c=false;if(!d(this,3,'gem')){break a}return false}this._=this.A-g;return!b(this,'')?false:!j(this)?false:true};a.prototype.r_en_ending=a.prototype.T;function p(c){var g;var h;var e;var i;var k;if(!(!(c.I_p1<=c._)?false:true)){return false}g=c.A-c._;if(!f(c,a.g_v,97,232)){return false}k=c._=(i=c.A)-g;h=i-k;e=true;a:while(e===true){e=false;if(!d(c,3,'gem')){break a}return false}c._=c.A-h;return!b(c,'')?false:!j(c)?false:true};a.prototype.X=function(){var c;var v;var w;var x;var y;var z;var A;var B;var C;var D;var M;var m;var g;var i;var k;var l;var e;var n;var q;var r;var s;var E;var F;var G;var H;var I;var J;var K;var L;var t;var N;var u;v=this.A-this._;m=true;a:while(m===true){m=false;this.B=this._;c=h(this,a.a_3,5);if(c===0){break a}this.C=this._;switch(c){case 0:break a;case 1:if(!(!(this.I_p1<=this._)?false:true)){break a}if(!b(this,'heid')){return false}break;case 2:if(!p(this)){break a}break;case 3:if(!(!(this.I_p1<=this._)?false:true)){break a}if(!f(this,a.g_v_j,97,232)){break a}if(!b(this,'')){return false}break}}F=this._=(E=this.A)-v;w=E-F;g=true;a:while(g===true){g=false;if(!o(this)){break a}}I=this._=(H=this.A)-w;x=H-I;i=true;a:while(i===true){i=false;this.B=this._;if(!d(this,4,'heid')){break a}this.C=G=this._;if(!(!(this.I_p2<=G)?false:true)){break a}y=this.A-this._;k=true;b:while(k===true){k=false;if(!d(this,1,'c')){break b}break a}this._=this.A-y;if(!b(this,'')){return false}this.B=this._;if(!d(this,2,'en')){break a}this.C=this._;if(!p(this)){break a}}L=this._=(K=this.A)-x;z=K-L;l=true;a:while(l===true){l=false;this.B=this._;c=h(this,a.a_4,6);if(c===0){break a}this.C=this._;switch(c){case 0:break a;case 1:if(!(!(this.I_p2<=this._)?false:true)){break a}if(!b(this,'')){return false}e=true;c:while(e===true){e=false;A=this.A-this._;n=true;b:while(n===true){n=false;this.B=this._;if(!d(this,2,'ig')){break b}this.C=J=this._;if(!(!(this.I_p2<=J)?false:true)){break b}B=this.A-this._;q=true;d:while(q===true){q=false;if(!d(this,1,'e')){break d}break b}this._=this.A-B;if(!b(this,'')){return false}break c}this._=this.A-A;if(!j(this)){break a}}break;case 2:if(!(!(this.I_p2<=this._)?false:true)){break a}C=this.A-this._;r=true;b:while(r===true){r=false;if(!d(this,1,'e')){break b}break a}this._=this.A-C;if(!b(this,'')){return false}break;case 3:if(!(!(this.I_p2<=this._)?false:true)){break a}if(!b(this,'')){return false}if(!o(this)){break a}break;case 4:if(!(!(this.I_p2<=this._)?false:true)){break a}if(!b(this,'')){return false}break;case 5:if(!(!(this.I_p2<=this._)?false:true)){break a}if(!this.B_e_found){break a}if(!b(this,'')){return false}break}}u=this._=(N=this.A)-z;D=N-u;s=true;a:while(s===true){s=false;if(!f(this,a.g_v_I,73,232)){break a}M=this.A-this._;if(h(this,a.a_5,4)===0){break a}if(!f(this,a.g_v,97,232)){break a}t=this._=this.A-M;this.B=t;if(t<=this.E){break a}this._--;this.C=this._;if(!b(this,'')){return false}}this._=this.A-D;return true};a.prototype.r_standard_suffix=a.prototype.X;function J(c){var e;var w;var x;var y;var z;var A;var B;var C;var D;var E;var N;var g;var i;var k;var l;var m;var n;var q;var r;var s;var t;var F;var G;var H;var I;var J;var K;var L;var M;var u;var O;var v;w=c.A-c._;g=true;a:while(g===true){g=false;c.B=c._;e=h(c,a.a_3,5);if(e===0){break a}c.C=c._;switch(e){case 0:break a;case 1:if(!(!(c.I_p1<=c._)?false:true)){break a}if(!b(c,'heid')){return false}break;case 2:if(!p(c)){break a}break;case 3:if(!(!(c.I_p1<=c._)?false:true)){break a}if(!f(c,a.g_v_j,97,232)){break a}if(!b(c,'')){return false}break}}G=c._=(F=c.A)-w;x=F-G;i=true;a:while(i===true){i=false;if(!o(c)){break a}}J=c._=(I=c.A)-x;y=I-J;k=true;a:while(k===true){k=false;c.B=c._;if(!d(c,4,'heid')){break a}c.C=H=c._;if(!(!(c.I_p2<=H)?false:true)){break a}z=c.A-c._;l=true;b:while(l===true){l=false;if(!d(c,1,'c')){break b}break a}c._=c.A-z;if(!b(c,'')){return false}c.B=c._;if(!d(c,2,'en')){break a}c.C=c._;if(!p(c)){break a}}M=c._=(L=c.A)-y;A=L-M;m=true;a:while(m===true){m=false;c.B=c._;e=h(c,a.a_4,6);if(e===0){break a}c.C=c._;switch(e){case 0:break a;case 1:if(!(!(c.I_p2<=c._)?false:true)){break a}if(!b(c,'')){return false}n=true;c:while(n===true){n=false;B=c.A-c._;q=true;b:while(q===true){q=false;c.B=c._;if(!d(c,2,'ig')){break b}c.C=K=c._;if(!(!(c.I_p2<=K)?false:true)){break b}C=c.A-c._;r=true;d:while(r===true){r=false;if(!d(c,1,'e')){break d}break b}c._=c.A-C;if(!b(c,'')){return false}break c}c._=c.A-B;if(!j(c)){break a}}break;case 2:if(!(!(c.I_p2<=c._)?false:true)){break a}D=c.A-c._;s=true;b:while(s===true){s=false;if(!d(c,1,'e')){break b}break a}c._=c.A-D;if(!b(c,'')){return false}break;case 3:if(!(!(c.I_p2<=c._)?false:true)){break a}if(!b(c,'')){return false}if(!o(c)){break a}break;case 4:if(!(!(c.I_p2<=c._)?false:true)){break a}if(!b(c,'')){return false}break;case 5:if(!(!(c.I_p2<=c._)?false:true)){break a}if(!c.B_e_found){break a}if(!b(c,'')){return false}break}}v=c._=(O=c.A)-A;E=O-v;t=true;a:while(t===true){t=false;if(!f(c,a.g_v_I,73,232)){break a}N=c.A-c._;if(h(c,a.a_5,4)===0){break a}if(!f(c,a.g_v,97,232)){break a}u=c._=c.A-N;c.B=u;if(u<=c.E){break a}c._--;c.C=c._;if(!b(c,'')){return false}}c._=c.A-E;return true};a.prototype.J=function(){var f;var g;var h;var b;var a;var c;var d;var i;var j;var e;f=this._;b=true;a:while(b===true){b=false;if(!F(this)){break a}}i=this._=f;g=i;a=true;a:while(a===true){a=false;if(!G(this)){break a}}j=this._=g;this.E=j;this._=this.A;c=true;a:while(c===true){c=false;if(!J(this)){break a}}e=this._=this.E;h=e;d=true;a:while(d===true){d=false;if(!H(this)){break a}}this._=h;return true};a.prototype.stem=a.prototype.J;a.prototype.N=function(b){return b instanceof a};a.prototype.equals=a.prototype.N;a.prototype.O=function(){var c;var a;var b;var d;c='DutchStemmer';a=0;for(b=0;b<c.length;b++){d=c.charCodeAt(b);a=(a<<5)-a+d;a=a&a}return a|0};a.prototype.hashCode=a.prototype.O;a.serialVersionUID=1;e(a,'methodObject',function(){return new a});e(a,'a_0',function(){return[new c('',-1,6),new c('á',0,1),new c('ä',0,1),new c('é',0,2),new c('ë',0,2),new c('í',0,3),new c('ï',0,3),new c('ó',0,4),new c('ö',0,4),new c('ú',0,5),new c('ü',0,5)]});e(a,'a_1',function(){return[new c('',-1,3),new c('I',0,2),new c('Y',0,1)]});e(a,'a_2',function(){return[new c('dd',-1,-1),new c('kk',-1,-1),new c('tt',-1,-1)]});e(a,'a_3',function(){return[new c('ene',-1,2),new c('se',-1,3),new c('en',-1,2),new c('heden',2,1),new c('s',-1,3)]});e(a,'a_4',function(){return[new c('end',-1,1),new c('ig',-1,2),new c('ing',-1,1),new c('lijk',-1,3),new c('baar',-1,4),new c('bar',-1,5)]});e(a,'a_5',function(){return[new c('aa',-1,-1),new c('ee',-1,-1),new c('oo',-1,-1),new c('uu',-1,-1)]});e(a,'g_v',function(){return[17,65,16,1,0,0,0,0,0,0,0,0,0,0,0,0,128]});e(a,'g_v_I',function(){return[1,0,0,17,65,16,1,0,0,0,0,0,0,0,0,0,0,0,0,128]});e(a,'g_v_j',function(){return[17,67,16,1,0,0,0,0,0,0,0,0,0,0,0,0,128]});var t={'src/stemmer.jsx':{Stemmer:s},'src/dutch-stemmer.jsx':{DutchStemmer:a}}}(JSX))
var Stemmer = JSX.require("src/dutch-stemmer.jsx").DutchStemmer;



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
    var stopwords = ["aan","al","alles","als","altijd","andere","ben","bij","daar","dan","dat","de","der","deze","die","dit","doch","doen","door","dus","een","eens","en","er","ge","geen","geweest","haar","had","heb","hebben","heeft","hem","het","hier","hij","hoe","hun","iemand","iets","ik","in","is","ja","je","kan","kon","kunnen","maar","me","meer","men","met","mij","mijn","moet","na","naar","niet","niets","nog","nu","of","om","omdat","onder","ons","ook","op","over","reeds","te","tegen","toch","toen","tot","u","uit","uw","van","veel","voor","want","waren","was","wat","werd","wezen","wie","wil","worden","wordt","zal","ze","zelf","zich","zij","zijn","zo","zonder","zou"];

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