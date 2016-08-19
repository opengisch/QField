/*
 * searchtools.js_t
 * ~~~~~~~~~~~~~~~~
 *
 * Sphinx JavaScript utilties for the full-text search.
 *
 * :copyright: Copyright 2007-2015 by the Sphinx team, see AUTHORS.
 * :license: BSD, see LICENSE for details.
 *
 */


var JSX={};(function(j){function l(b,e){var a=function(){};a.prototype=e.prototype;var c=new a;for(var d in b){b[d].prototype=c}}function L(c,b){for(var a in b.prototype)if(b.prototype.hasOwnProperty(a))c.prototype[a]=b.prototype[a]}function h(a,b,d){function c(a,b,c){delete a[b];a[b]=c;return c}Object.defineProperty(a,b,{get:function(){return c(a,b,d())},set:function(d){c(a,b,d)},enumerable:true,configurable:true})}function M(a,b,c){return a[b]=a[b]/c|0}var E=parseInt;var C=parseFloat;function N(a){return a!==a}var A=isFinite;var z=encodeURIComponent;var y=decodeURIComponent;var x=encodeURI;var w=decodeURI;var u=Object.prototype.toString;var D=Object.prototype.hasOwnProperty;function k(){}j.require=function(b){var a=r[b];return a!==undefined?a:null};j.profilerIsRunning=function(){return k.getResults!=null};j.getProfileResults=function(){return(k.getResults||function(){return{}})()};j.postProfileResults=function(a,b){if(k.postResults==null)throw new Error('profiler has not been turned on');return k.postResults(a,b)};j.resetProfileResults=function(){if(k.resetResults==null)throw new Error('profiler has not been turned on');return k.resetResults()};j.DEBUG=false;function t(){};l([t],Error);function a(a,b,c){this.F=a.length;this.K=a;this.L=b;this.I=c;this.H=null;this.P=null};l([a],Object);function n(){};l([n],Object);function g(){var a;var b;var c;this.G={};a=this.E='';b=this._=0;c=this.A=a.length;this.D=0;this.B=b;this.C=c};l([g],n);function v(a,b){a.E=b.E;a._=b._;a.A=b.A;a.D=b.D;a.B=b.B;a.C=b.C};function d(b,d,c,e){var a;if(b._>=b.A){return false}a=b.E.charCodeAt(b._);if(a>e||a<c){return false}a-=c;if((d[a>>>3]&1<<(a&7))===0){return false}b._++;return true};function e(a,d,c,e){var b;if(a._>=a.A){return false}b=a.E.charCodeAt(a._);if(b>e||b<c){a._++;return true}b-=c;if((d[b>>>3]&1<<(b&7))===0){a._++;return true}return false};function p(a,d,c,e){var b;if(a._<=a.D){return false}b=a.E.charCodeAt(a._-1);if(b>e||b<c){a._--;return true}b-=c;if((d[b>>>3]&1<<(b&7))===0){a._--;return true}return false};function m(a,b,d){var c;if(a.A-a._<b){return false}if(a.E.slice(c=a._,c+b)!==d){return false}a._+=b;return true};function i(a,b,d){var c;if(a._-a.D<b){return false}if(a.E.slice((c=a._)-b,c)!==d){return false}a._-=b;return true};function q(f,m,p){var b;var d;var e;var n;var g;var k;var l;var i;var h;var c;var a;var j;var o;b=0;d=p;e=f._;n=f.A;g=0;k=0;l=false;while(true){i=b+(d-b>>>1);h=0;c=g<k?g:k;a=m[i];for(j=c;j<a.F;j++){if(e+c===n){h=-1;break}h=f.E.charCodeAt(e+c)-a.K.charCodeAt(j);if(h!==0){break}c++}if(h<0){d=i;k=c}else{b=i;g=c}if(d-b<=1){if(b>0){break}if(d===b){break}if(l){break}l=true}}while(true){a=m[b];if(g>=a.F){f._=e+a.F|0;if(a.H==null){return a.I}o=a.H(a.P);f._=e+a.F|0;if(o){return a.I}}b=a.L;if(b<0){return 0}}return-1};function f(d,m,p){var b;var g;var e;var n;var f;var k;var l;var i;var h;var c;var a;var j;var o;b=0;g=p;e=d._;n=d.D;f=0;k=0;l=false;while(true){i=b+(g-b>>1);h=0;c=f<k?f:k;a=m[i];for(j=a.F-1-c;j>=0;j--){if(e-c===n){h=-1;break}h=d.E.charCodeAt(e-1-c)-a.K.charCodeAt(j);if(h!==0){break}c++}if(h<0){g=i;k=c}else{b=i;f=c}if(g-b<=1){if(b>0){break}if(g===b){break}if(l){break}l=true}}while(true){a=m[b];if(f>=a.F){d._=e-a.F|0;if(a.H==null){return a.I}o=a.H(d);d._=e-a.F|0;if(o){return a.I}}b=a.L;if(b<0){return 0}}return-1};function s(a,b,d,e){var c;c=e.length-(d-b);a.E=a.E.slice(0,b)+e+a.E.slice(d);a.A+=c|0;if(a._>=d){a._+=c|0}else if(a._>b){a._=b}return c|0};function c(a,f){var b;var c;var d;var e;b=false;if((c=a.B)<0||c>(d=a.C)||d>(e=a.A)||e>a.E.length?false:true){s(a,a.B,a.C,f);b=true}return b};g.prototype.J=function(){return false};g.prototype.b=function(b){var a;var c;var d;var e;a=this.G['.'+b];if(a==null){c=this.E=b;d=this._=0;e=this.A=c.length;this.D=0;this.B=d;this.C=e;this.J();a=this.E;this.G['.'+b]=a}return a};g.prototype.stemWord=g.prototype.b;g.prototype.c=function(e){var d;var b;var c;var a;var f;var g;var h;d=[];for(b=0;b<e.length;b++){c=e[b];a=this.G['.'+c];if(a==null){f=this.E=c;g=this._=0;h=this.A=f.length;this.D=0;this.B=g;this.C=h;this.J();a=this.E;this.G['.'+c]=a}d.push(a)}return d};g.prototype.stemWords=g.prototype.c;function b(){g.call(this);this.B_standard_suffix_removed=false;this.I_p2=0;this.I_p1=0;this.I_pV=0};l([b],g);b.prototype.M=function(a){this.B_standard_suffix_removed=a.B_standard_suffix_removed;this.I_p2=a.I_p2;this.I_p1=a.I_p1;this.I_pV=a.I_pV;v(this,a)};b.prototype.copy_from=b.prototype.M;b.prototype.W=function(){var i;var a;var j;var e;var f;var g;var h;var k;b:while(true){i=this._;e=true;d:while(e===true){e=false;e:while(true){a=this._;f=true;a:while(f===true){f=false;if(!d(this,b.g_v,97,259)){break a}this.B=this._;g=true;f:while(g===true){g=false;j=this._;h=true;c:while(h===true){h=false;if(!m(this,1,'u')){break c}this.C=this._;if(!d(this,b.g_v,97,259)){break c}if(!c(this,'U')){return false}break f}this._=j;if(!m(this,1,'i')){break a}this.C=this._;if(!d(this,b.g_v,97,259)){break a}if(!c(this,'I')){return false}}this._=a;break e}k=this._=a;if(k>=this.A){break d}this._++}continue b}this._=i;break b}return true};b.prototype.r_prelude=b.prototype.W;function G(a){var j;var e;var k;var f;var g;var h;var i;var l;b:while(true){j=a._;f=true;d:while(f===true){f=false;e:while(true){e=a._;g=true;a:while(g===true){g=false;if(!d(a,b.g_v,97,259)){break a}a.B=a._;h=true;f:while(h===true){h=false;k=a._;i=true;c:while(i===true){i=false;if(!m(a,1,'u')){break c}a.C=a._;if(!d(a,b.g_v,97,259)){break c}if(!c(a,'U')){return false}break f}a._=k;if(!m(a,1,'i')){break a}a.C=a._;if(!d(a,b.g_v,97,259)){break a}if(!c(a,'I')){return false}}a._=e;break e}l=a._=e;if(l>=a.A){break d}a._++}continue b}a._=j;break b}return true};b.prototype.U=function(){var u;var w;var x;var y;var t;var l;var f;var g;var h;var i;var c;var j;var k;var a;var m;var n;var o;var p;var q;var r;var s;var v;this.I_pV=s=this.A;this.I_p1=s;this.I_p2=s;u=this._;l=true;a:while(l===true){l=false;f=true;g:while(f===true){f=false;w=this._;g=true;b:while(g===true){g=false;if(!d(this,b.g_v,97,259)){break b}h=true;f:while(h===true){h=false;x=this._;i=true;c:while(i===true){i=false;if(!e(this,b.g_v,97,259)){break c}d:while(true){c=true;e:while(c===true){c=false;if(!d(this,b.g_v,97,259)){break e}break d}if(this._>=this.A){break c}this._++}break f}this._=x;if(!d(this,b.g_v,97,259)){break b}c:while(true){j=true;d:while(j===true){j=false;if(!e(this,b.g_v,97,259)){break d}break c}if(this._>=this.A){break b}this._++}}break g}this._=w;if(!e(this,b.g_v,97,259)){break a}k=true;c:while(k===true){k=false;y=this._;a=true;b:while(a===true){a=false;if(!e(this,b.g_v,97,259)){break b}e:while(true){m=true;d:while(m===true){m=false;if(!d(this,b.g_v,97,259)){break d}break e}if(this._>=this.A){break b}this._++}break c}this._=y;if(!d(this,b.g_v,97,259)){break a}if(this._>=this.A){break a}this._++}}this.I_pV=this._}v=this._=u;t=v;n=true;a:while(n===true){n=false;b:while(true){o=true;c:while(o===true){o=false;if(!d(this,b.g_v,97,259)){break c}break b}if(this._>=this.A){break a}this._++}b:while(true){p=true;c:while(p===true){p=false;if(!e(this,b.g_v,97,259)){break c}break b}if(this._>=this.A){break a}this._++}this.I_p1=this._;b:while(true){q=true;c:while(q===true){q=false;if(!d(this,b.g_v,97,259)){break c}break b}if(this._>=this.A){break a}this._++}c:while(true){r=true;b:while(r===true){r=false;if(!e(this,b.g_v,97,259)){break b}break c}if(this._>=this.A){break a}this._++}this.I_p2=this._}this._=t;return true};b.prototype.r_mark_regions=b.prototype.U;function H(a){var x;var y;var z;var u;var v;var l;var f;var g;var h;var i;var j;var k;var c;var m;var n;var o;var p;var q;var r;var s;var t;var w;a.I_pV=t=a.A;a.I_p1=t;a.I_p2=t;x=a._;l=true;a:while(l===true){l=false;f=true;g:while(f===true){f=false;y=a._;g=true;b:while(g===true){g=false;if(!d(a,b.g_v,97,259)){break b}h=true;f:while(h===true){h=false;z=a._;i=true;c:while(i===true){i=false;if(!e(a,b.g_v,97,259)){break c}d:while(true){j=true;e:while(j===true){j=false;if(!d(a,b.g_v,97,259)){break e}break d}if(a._>=a.A){break c}a._++}break f}a._=z;if(!d(a,b.g_v,97,259)){break b}c:while(true){k=true;d:while(k===true){k=false;if(!e(a,b.g_v,97,259)){break d}break c}if(a._>=a.A){break b}a._++}}break g}a._=y;if(!e(a,b.g_v,97,259)){break a}c=true;c:while(c===true){c=false;u=a._;m=true;b:while(m===true){m=false;if(!e(a,b.g_v,97,259)){break b}e:while(true){n=true;d:while(n===true){n=false;if(!d(a,b.g_v,97,259)){break d}break e}if(a._>=a.A){break b}a._++}break c}a._=u;if(!d(a,b.g_v,97,259)){break a}if(a._>=a.A){break a}a._++}}a.I_pV=a._}w=a._=x;v=w;o=true;a:while(o===true){o=false;b:while(true){p=true;c:while(p===true){p=false;if(!d(a,b.g_v,97,259)){break c}break b}if(a._>=a.A){break a}a._++}b:while(true){q=true;c:while(q===true){q=false;if(!e(a,b.g_v,97,259)){break c}break b}if(a._>=a.A){break a}a._++}a.I_p1=a._;b:while(true){r=true;c:while(r===true){r=false;if(!d(a,b.g_v,97,259)){break c}break b}if(a._>=a.A){break a}a._++}c:while(true){s=true;b:while(s===true){s=false;if(!e(a,b.g_v,97,259)){break b}break c}if(a._>=a.A){break a}a._++}a.I_p2=a._}a._=v;return true};b.prototype.V=function(){var a;var e;var d;b:while(true){e=this._;d=true;a:while(d===true){d=false;this.B=this._;a=q(this,b.a_0,3);if(a===0){break a}this.C=this._;switch(a){case 0:break a;case 1:if(!c(this,'i')){return false}break;case 2:if(!c(this,'u')){return false}break;case 3:if(this._>=this.A){break a}this._++;break}continue b}this._=e;break b}return true};b.prototype.r_postlude=b.prototype.V;function I(a){var d;var f;var e;b:while(true){f=a._;e=true;a:while(e===true){e=false;a.B=a._;d=q(a,b.a_0,3);if(d===0){break a}a.C=a._;switch(d){case 0:break a;case 1:if(!c(a,'i')){return false}break;case 2:if(!c(a,'u')){return false}break;case 3:if(a._>=a.A){break a}a._++;break}continue b}a._=f;break b}return true};b.prototype.S=function(){return!(this.I_pV<=this._)?false:true};b.prototype.r_RV=b.prototype.S;b.prototype.Q=function(){return!(this.I_p1<=this._)?false:true};b.prototype.r_R1=b.prototype.Q;b.prototype.R=function(){return!(this.I_p2<=this._)?false:true};b.prototype.r_R2=b.prototype.R;b.prototype.Y=function(){var a;var e;var d;var g;this.C=this._;a=f(this,b.a_1,16);if(a===0){return false}this.B=g=this._;if(!(!(this.I_p1<=g)?false:true)){return false}switch(a){case 0:return false;case 1:if(!c(this,'')){return false}break;case 2:if(!c(this,'a')){return false}break;case 3:if(!c(this,'e')){return false}break;case 4:if(!c(this,'i')){return false}break;case 5:e=this.A-this._;d=true;a:while(d===true){d=false;if(!i(this,2,'ab')){break a}return false}this._=this.A-e;if(!c(this,'i')){return false}break;case 6:if(!c(this,'at')){return false}break;case 7:if(!c(this,'aţi')){return false}break}return true};b.prototype.r_step_0=b.prototype.Y;function J(a){var d;var g;var e;var h;a.C=a._;d=f(a,b.a_1,16);if(d===0){return false}a.B=h=a._;if(!(!(a.I_p1<=h)?false:true)){return false}switch(d){case 0:return false;case 1:if(!c(a,'')){return false}break;case 2:if(!c(a,'a')){return false}break;case 3:if(!c(a,'e')){return false}break;case 4:if(!c(a,'i')){return false}break;case 5:g=a.A-a._;e=true;a:while(e===true){e=false;if(!i(a,2,'ab')){break a}return false}a._=a.A-g;if(!c(a,'i')){return false}break;case 6:if(!c(a,'at')){return false}break;case 7:if(!c(a,'aţi')){return false}break}return true};b.prototype.T=function(){var a;var d;var e;var g;d=this.A-(e=this._);this.C=e;a=f(this,b.a_2,46);if(a===0){return false}this.B=g=this._;if(!(!(this.I_p1<=g)?false:true)){return false}switch(a){case 0:return false;case 1:if(!c(this,'abil')){return false}break;case 2:if(!c(this,'ibil')){return false}break;case 3:if(!c(this,'iv')){return false}break;case 4:if(!c(this,'ic')){return false}break;case 5:if(!c(this,'at')){return false}break;case 6:if(!c(this,'it')){return false}break}this.B_standard_suffix_removed=true;this._=this.A-d;return true};b.prototype.r_combo_suffix=b.prototype.T;function o(a){var d;var e;var g;var h;e=a.A-(g=a._);a.C=g;d=f(a,b.a_2,46);if(d===0){return false}a.B=h=a._;if(!(!(a.I_p1<=h)?false:true)){return false}switch(d){case 0:return false;case 1:if(!c(a,'abil')){return false}break;case 2:if(!c(a,'ibil')){return false}break;case 3:if(!c(a,'iv')){return false}break;case 4:if(!c(a,'ic')){return false}break;case 5:if(!c(a,'at')){return false}break;case 6:if(!c(a,'it')){return false}break}a.B_standard_suffix_removed=true;a._=a.A-e;return true};b.prototype.X=function(){var a;var e;var d;var g;this.B_standard_suffix_removed=false;a:while(true){e=this.A-this._;d=true;b:while(d===true){d=false;if(!o(this)){break b}continue a}this._=this.A-e;break a}this.C=this._;a=f(this,b.a_3,62);if(a===0){return false}this.B=g=this._;if(!(!(this.I_p2<=g)?false:true)){return false}switch(a){case 0:return false;case 1:if(!c(this,'')){return false}break;case 2:if(!i(this,1,'ţ')){return false}this.B=this._;if(!c(this,'t')){return false}break;case 3:if(!c(this,'ist')){return false}break}this.B_standard_suffix_removed=true;return true};b.prototype.r_standard_suffix=b.prototype.X;function K(a){var d;var g;var e;var h;a.B_standard_suffix_removed=false;a:while(true){g=a.A-a._;e=true;b:while(e===true){e=false;if(!o(a)){break b}continue a}a._=a.A-g;break a}a.C=a._;d=f(a,b.a_3,62);if(d===0){return false}a.B=h=a._;if(!(!(a.I_p2<=h)?false:true)){return false}switch(d){case 0:return false;case 1:if(!c(a,'')){return false}break;case 2:if(!i(a,1,'ţ')){return false}a.B=a._;if(!c(a,'t')){return false}break;case 3:if(!c(a,'ist')){return false}break}a.B_standard_suffix_removed=true;return true};b.prototype.Z=function(){var d;var h;var a;var j;var e;var g;var k;var l;var m;h=this.A-(k=this._);if(k<this.I_pV){return false}l=this._=this.I_pV;a=this.D;this.D=l;m=this._=this.A-h;this.C=m;d=f(this,b.a_4,94);if(d===0){this.D=a;return false}this.B=this._;switch(d){case 0:this.D=a;return false;case 1:e=true;a:while(e===true){e=false;j=this.A-this._;g=true;b:while(g===true){g=false;if(!p(this,b.g_v,97,259)){break b}break a}this._=this.A-j;if(!i(this,1,'u')){this.D=a;return false}}if(!c(this,'')){return false}break;case 2:if(!c(this,'')){return false}break}this.D=a;return true};b.prototype.r_verb_suffix=b.prototype.Z;function F(a){var e;var l;var d;var j;var g;var h;var m;var n;var k;l=a.A-(m=a._);if(m<a.I_pV){return false}n=a._=a.I_pV;d=a.D;a.D=n;k=a._=a.A-l;a.C=k;e=f(a,b.a_4,94);if(e===0){a.D=d;return false}a.B=a._;switch(e){case 0:a.D=d;return false;case 1:g=true;a:while(g===true){g=false;j=a.A-a._;h=true;b:while(h===true){h=false;if(!p(a,b.g_v,97,259)){break b}break a}a._=a.A-j;if(!i(a,1,'u')){a.D=d;return false}}if(!c(a,'')){return false}break;case 2:if(!c(a,'')){return false}break}a.D=d;return true};b.prototype.a=function(){var a;var d;this.C=this._;a=f(this,b.a_5,5);if(a===0){return false}this.B=d=this._;if(!(!(this.I_pV<=d)?false:true)){return false}switch(a){case 0:return false;case 1:if(!c(this,'')){return false}break}return true};b.prototype.r_vowel_suffix=b.prototype.a;function B(a){var d;var e;a.C=a._;d=f(a,b.a_5,5);if(d===0){return false}a.B=e=a._;if(!(!(a.I_pV<=e)?false:true)){return false}switch(d){case 0:return false;case 1:if(!c(a,'')){return false}break}return true};b.prototype.J=function(){var n;var j;var k;var l;var m;var o;var p;var b;var c;var d;var e;var f;var a;var g;var h;var i;var r;var s;var t;var u;var v;var w;var x;var y;var q;n=this._;b=true;a:while(b===true){b=false;if(!G(this)){break a}}r=this._=n;j=r;c=true;a:while(c===true){c=false;if(!H(this)){break a}}s=this._=j;this.D=s;u=this._=t=this.A;k=t-u;d=true;a:while(d===true){d=false;if(!J(this)){break a}}w=this._=(v=this.A)-k;l=v-w;e=true;a:while(e===true){e=false;if(!K(this)){break a}}y=this._=(x=this.A)-l;m=x-y;f=true;a:while(f===true){f=false;a=true;b:while(a===true){a=false;o=this.A-this._;g=true;c:while(g===true){g=false;if(!this.B_standard_suffix_removed){break c}break b}this._=this.A-o;if(!F(this)){break a}}}this._=this.A-m;h=true;a:while(h===true){h=false;if(!B(this)){break a}}q=this._=this.D;p=q;i=true;a:while(i===true){i=false;if(!I(this)){break a}}this._=p;return true};b.prototype.stem=b.prototype.J;b.prototype.N=function(a){return a instanceof b};b.prototype.equals=b.prototype.N;b.prototype.O=function(){var c;var a;var b;var d;c='RomanianStemmer';a=0;for(b=0;b<c.length;b++){d=c.charCodeAt(b);a=(a<<5)-a+d;a=a&a}return a|0};b.prototype.hashCode=b.prototype.O;b.serialVersionUID=1;h(b,'methodObject',function(){return new b});h(b,'a_0',function(){return[new a('',-1,3),new a('I',0,1),new a('U',0,2)]});h(b,'a_1',function(){return[new a('ea',-1,3),new a('aţia',-1,7),new a('aua',-1,2),new a('iua',-1,4),new a('aţie',-1,7),new a('ele',-1,3),new a('ile',-1,5),new a('iile',6,4),new a('iei',-1,4),new a('atei',-1,6),new a('ii',-1,4),new a('ului',-1,1),new a('ul',-1,1),new a('elor',-1,3),new a('ilor',-1,4),new a('iilor',14,4)]});h(b,'a_2',function(){return[new a('icala',-1,4),new a('iciva',-1,4),new a('ativa',-1,5),new a('itiva',-1,6),new a('icale',-1,4),new a('aţiune',-1,5),new a('iţiune',-1,6),new a('atoare',-1,5),new a('itoare',-1,6),new a('ătoare',-1,5),new a('icitate',-1,4),new a('abilitate',-1,1),new a('ibilitate',-1,2),new a('ivitate',-1,3),new a('icive',-1,4),new a('ative',-1,5),new a('itive',-1,6),new a('icali',-1,4),new a('atori',-1,5),new a('icatori',18,4),new a('itori',-1,6),new a('ători',-1,5),new a('icitati',-1,4),new a('abilitati',-1,1),new a('ivitati',-1,3),new a('icivi',-1,4),new a('ativi',-1,5),new a('itivi',-1,6),new a('icităi',-1,4),new a('abilităi',-1,1),new a('ivităi',-1,3),new a('icităţi',-1,4),new a('abilităţi',-1,1),new a('ivităţi',-1,3),new a('ical',-1,4),new a('ator',-1,5),new a('icator',35,4),new a('itor',-1,6),new a('ător',-1,5),new a('iciv',-1,4),new a('ativ',-1,5),new a('itiv',-1,6),new a('icală',-1,4),new a('icivă',-1,4),new a('ativă',-1,5),new a('itivă',-1,6)]});h(b,'a_3',function(){return[new a('ica',-1,1),new a('abila',-1,1),new a('ibila',-1,1),new a('oasa',-1,1),new a('ata',-1,1),new a('ita',-1,1),new a('anta',-1,1),new a('ista',-1,3),new a('uta',-1,1),new a('iva',-1,1),new a('ic',-1,1),new a('ice',-1,1),new a('abile',-1,1),new a('ibile',-1,1),new a('isme',-1,3),new a('iune',-1,2),new a('oase',-1,1),new a('ate',-1,1),new a('itate',17,1),new a('ite',-1,1),new a('ante',-1,1),new a('iste',-1,3),new a('ute',-1,1),new a('ive',-1,1),new a('ici',-1,1),new a('abili',-1,1),new a('ibili',-1,1),new a('iuni',-1,2),new a('atori',-1,1),new a('osi',-1,1),new a('ati',-1,1),new a('itati',30,1),new a('iti',-1,1),new a('anti',-1,1),new a('isti',-1,3),new a('uti',-1,1),new a('işti',-1,3),new a('ivi',-1,1),new a('ităi',-1,1),new a('oşi',-1,1),new a('ităţi',-1,1),new a('abil',-1,1),new a('ibil',-1,1),new a('ism',-1,3),new a('ator',-1,1),new a('os',-1,1),new a('at',-1,1),new a('it',-1,1),new a('ant',-1,1),new a('ist',-1,3),new a('ut',-1,1),new a('iv',-1,1),new a('ică',-1,1),new a('abilă',-1,1),new a('ibilă',-1,1),new a('oasă',-1,1),new a('ată',-1,1),new a('ită',-1,1),new a('antă',-1,1),new a('istă',-1,3),new a('ută',-1,1),new a('ivă',-1,1)]});h(b,'a_4',function(){return[new a('ea',-1,1),new a('ia',-1,1),new a('esc',-1,1),new a('ăsc',-1,1),new a('ind',-1,1),new a('ând',-1,1),new a('are',-1,1),new a('ere',-1,1),new a('ire',-1,1),new a('âre',-1,1),new a('se',-1,2),new a('ase',10,1),new a('sese',10,2),new a('ise',10,1),new a('use',10,1),new a('âse',10,1),new a('eşte',-1,1),new a('ăşte',-1,1),new a('eze',-1,1),new a('ai',-1,1),new a('eai',19,1),new a('iai',19,1),new a('sei',-1,2),new a('eşti',-1,1),new a('ăşti',-1,1),new a('ui',-1,1),new a('ezi',-1,1),new a('âi',-1,1),new a('aşi',-1,1),new a('seşi',-1,2),new a('aseşi',29,1),new a('seseşi',29,2),new a('iseşi',29,1),new a('useşi',29,1),new a('âseşi',29,1),new a('işi',-1,1),new a('uşi',-1,1),new a('âşi',-1,1),new a('aţi',-1,2),new a('eaţi',38,1),new a('iaţi',38,1),new a('eţi',-1,2),new a('iţi',-1,2),new a('âţi',-1,2),new a('arăţi',-1,1),new a('serăţi',-1,2),new a('aserăţi',45,1),new a('seserăţi',45,2),new a('iserăţi',45,1),new a('userăţi',45,1),new a('âserăţi',45,1),new a('irăţi',-1,1),new a('urăţi',-1,1),new a('ârăţi',-1,1),new a('am',-1,1),new a('eam',54,1),new a('iam',54,1),new a('em',-1,2),new a('asem',57,1),new a('sesem',57,2),new a('isem',57,1),new a('usem',57,1),new a('âsem',57,1),new a('im',-1,2),new a('âm',-1,2),new a('ăm',-1,2),new a('arăm',65,1),new a('serăm',65,2),new a('aserăm',67,1),new a('seserăm',67,2),new a('iserăm',67,1),new a('userăm',67,1),new a('âserăm',67,1),new a('irăm',65,1),new a('urăm',65,1),new a('ârăm',65,1),new a('au',-1,1),new a('eau',76,1),new a('iau',76,1),new a('indu',-1,1),new a('ându',-1,1),new a('ez',-1,1),new a('ească',-1,1),new a('ară',-1,1),new a('seră',-1,2),new a('aseră',84,1),new a('seseră',84,2),new a('iseră',84,1),new a('useră',84,1),new a('âseră',84,1),new a('iră',-1,1),new a('ură',-1,1),new a('âră',-1,1),new a('ează',-1,1)]});h(b,'a_5',function(){return[new a('a',-1,1),new a('e',-1,1),new a('ie',1,1),new a('i',-1,1),new a('ă',-1,1)]});h(b,'g_v',function(){return[17,65,16,0,0,0,0,0,0,0,0,0,0,0,0,0,2,32,0,0,4]});var r={'src/stemmer.jsx':{Stemmer:n},'src/romanian-stemmer.jsx':{RomanianStemmer:b}}}(JSX))
var Stemmer = JSX.require("src/romanian-stemmer.jsx").RomanianStemmer;



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
    var stopwords = [];

    // stem the searchterms and add them to the correct list
    var stemmer = new Stemmer();
    var searchterms = [];
    var excluded = [];
    var hlterms = [];
    var tmp = query.split(/\s+/);
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
    results = results.concat(this.performTermsSearch(searchterms, excluded, terms, Scorer.term))
                     .concat(this.performTermsSearch(searchterms, excluded, titleterms, Scorer.title));

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
          $.ajax({url: DOCUMENTATION_OPTIONS.URL_ROOT + '_sources/' + item[0] + '.txt',
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
          results.push([filenames[match[0]], fullname, '#'+anchor, descr, score]);
        }
      }
    }

    return results;
  },

  /**
   * search for full-text terms in the index
   */
  performTermsSearch : function(searchterms, excluded, terms, score) {
    var filenames = this._index.filenames;
    var titles = this._index.titles;

    var i, j, file, files;
    var fileMap = {};
    var results = [];

    // perform the search on the required terms
    for (i = 0; i < searchterms.length; i++) {
      var word = searchterms[i];
      // no match but word was a required one
      if ((files = terms[word]) === undefined)
        break;
      if (files.length === undefined) {
        files = [files];
      }
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
          $u.contains(terms[excluded[i]] || [], file)) {
          valid = false;
          break;
        }
      }

      // if we have still a valid result we can add it to the result list
      if (valid) {
        results.push([filenames[file], titles[file], '', null, score]);
      }
    }
    return results;
  },

  /**
   * helper function to return a node containing the
   * search summary for a given text. keywords is a list
   * of stemmed words, hlwords is the list of normal, unstemmed
   * words. the first one is used to find the occurance, the
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