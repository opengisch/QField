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


var JSX={};(function(j){function l(b,e){var a=function(){};a.prototype=e.prototype;var c=new a;for(var d in b){b[d].prototype=c}}function H(c,b){for(var a in b.prototype)if(b.prototype.hasOwnProperty(a))c.prototype[a]=b.prototype[a]}function g(a,b,d){function c(a,b,c){delete a[b];a[b]=c;return c}Object.defineProperty(a,b,{get:function(){return c(a,b,d())},set:function(d){c(a,b,d)},enumerable:true,configurable:true})}function I(a,b,c){return a[b]=a[b]/c|0}var C=parseInt;var r=parseFloat;function J(a){return a!==a}var z=isFinite;var y=encodeURIComponent;var x=decodeURIComponent;var w=encodeURI;var u=decodeURI;var t=Object.prototype.toString;var B=Object.prototype.hasOwnProperty;function i(){}j.require=function(b){var a=q[b];return a!==undefined?a:null};j.profilerIsRunning=function(){return i.getResults!=null};j.getProfileResults=function(){return(i.getResults||function(){return{}})()};j.postProfileResults=function(a,b){if(i.postResults==null)throw new Error('profiler has not been turned on');return i.postResults(a,b)};j.resetProfileResults=function(){if(i.resetResults==null)throw new Error('profiler has not been turned on');return i.resetResults()};j.DEBUG=false;function s(){};l([s],Error);function c(a,b,c){this.F=a.length;this.K=a;this.L=b;this.I=c;this.H=null;this.P=null};l([c],Object);function o(){};l([o],Object);function e(){var a;var b;var c;this.G={};a=this.D='';b=this._=0;c=this.A=a.length;this.E=0;this.C=b;this.B=c};l([e],o);function v(a,b){a.D=b.D;a._=b._;a.A=b.A;a.E=b.E;a.C=b.C;a.B=b.B};function f(b,d,c,e){var a;if(b._>=b.A){return false}a=b.D.charCodeAt(b._);if(a>e||a<c){return false}a-=c;if((d[a>>>3]&1<<(a&7))===0){return false}b._++;return true};function m(b,d,c,e){var a;if(b._<=b.E){return false}a=b.D.charCodeAt(b._-1);if(a>e||a<c){return false}a-=c;if((d[a>>>3]&1<<(a&7))===0){return false}b._--;return true};function n(a,d,c,e){var b;if(a._>=a.A){return false}b=a.D.charCodeAt(a._);if(b>e||b<c){a._++;return true}b-=c;if((d[b>>>3]&1<<(b&7))===0){a._++;return true}return false};function k(a,b,d){var c;if(a.A-a._<b){return false}if(a.D.slice(c=a._,c+b)!==d){return false}a._+=b;return true};function d(a,b,d){var c;if(a._-a.E<b){return false}if(a.D.slice((c=a._)-b,c)!==d){return false}a._-=b;return true};function p(f,m,p){var b;var d;var e;var n;var g;var k;var l;var i;var h;var c;var a;var j;var o;b=0;d=p;e=f._;n=f.A;g=0;k=0;l=false;while(true){i=b+(d-b>>>1);h=0;c=g<k?g:k;a=m[i];for(j=c;j<a.F;j++){if(e+c===n){h=-1;break}h=f.D.charCodeAt(e+c)-a.K.charCodeAt(j);if(h!==0){break}c++}if(h<0){d=i;k=c}else{b=i;g=c}if(d-b<=1){if(b>0){break}if(d===b){break}if(l){break}l=true}}while(true){a=m[b];if(g>=a.F){f._=e+a.F|0;if(a.H==null){return a.I}o=a.H(a.P);f._=e+a.F|0;if(o){return a.I}}b=a.L;if(b<0){return 0}}return-1};function h(d,m,p){var b;var g;var e;var n;var f;var k;var l;var i;var h;var c;var a;var j;var o;b=0;g=p;e=d._;n=d.E;f=0;k=0;l=false;while(true){i=b+(g-b>>1);h=0;c=f<k?f:k;a=m[i];for(j=a.F-1-c;j>=0;j--){if(e-c===n){h=-1;break}h=d.D.charCodeAt(e-1-c)-a.K.charCodeAt(j);if(h!==0){break}c++}if(h<0){g=i;k=c}else{b=i;f=c}if(g-b<=1){if(b>0){break}if(g===b){break}if(l){break}l=true}}while(true){a=m[b];if(f>=a.F){d._=e-a.F|0;if(a.H==null){return a.I}o=a.H(d);d._=e-a.F|0;if(o){return a.I}}b=a.L;if(b<0){return 0}}return-1};function D(a,b,d,e){var c;c=e.length-(d-b);a.D=a.D.slice(0,b)+e+a.D.slice(d);a.A+=c|0;if(a._>=d){a._+=c|0}else if(a._>b){a._=b}return c|0};function b(a,f){var b;var c;var d;var e;b=false;if((c=a.C)<0||c>(d=a.B)||d>(e=a.A)||e>a.D.length?false:true){D(a,a.C,a.B,f);b=true}return b};e.prototype.J=function(){return false};e.prototype.W=function(b){var a;var c;var d;var e;a=this.G['.'+b];if(a==null){c=this.D=b;d=this._=0;e=this.A=c.length;this.E=0;this.C=d;this.B=e;this.J();a=this.D;this.G['.'+b]=a}return a};e.prototype.stemWord=e.prototype.W;e.prototype.X=function(e){var d;var b;var c;var a;var f;var g;var h;d=[];for(b=0;b<e.length;b++){c=e[b];a=this.G['.'+c];if(a==null){f=this.D=c;g=this._=0;h=this.A=f.length;this.E=0;this.C=g;this.B=h;this.J();a=this.D;this.G['.'+c]=a}d.push(a)}return d};e.prototype.stemWords=e.prototype.X;function a(){e.call(this);this.I_x=0;this.I_p2=0;this.I_p1=0};l([a],e);a.prototype.M=function(a){this.I_x=a.I_x;this.I_p2=a.I_p2;this.I_p1=a.I_p1;v(this,a)};a.prototype.copy_from=a.prototype.M;a.prototype.U=function(){var m;var r;var n;var o;var d;var q;var e;var c;var g;var h;var i;var j;var l;var s;var p;m=this._;a:while(true){r=this._;e=true;b:while(e===true){e=false;c=true;c:while(c===true){c=false;n=this._;g=true;d:while(g===true){g=false;this.C=this._;if(!k(this,1,'ß')){break d}this.B=this._;if(!b(this,'ss')){return false}break c}s=this._=n;if(s>=this.A){break b}this._++}continue a}this._=r;break a}this._=m;b:while(true){o=this._;h=true;d:while(h===true){h=false;e:while(true){d=this._;i=true;a:while(i===true){i=false;if(!f(this,a.g_v,97,252)){break a}this.C=this._;j=true;f:while(j===true){j=false;q=this._;l=true;c:while(l===true){l=false;if(!k(this,1,'u')){break c}this.B=this._;if(!f(this,a.g_v,97,252)){break c}if(!b(this,'U')){return false}break f}this._=q;if(!k(this,1,'y')){break a}this.B=this._;if(!f(this,a.g_v,97,252)){break a}if(!b(this,'Y')){return false}}this._=d;break e}p=this._=d;if(p>=this.A){break d}this._++}continue b}this._=o;break b}return true};a.prototype.r_prelude=a.prototype.U;function G(c){var s;var n;var o;var p;var e;var r;var d;var g;var h;var i;var j;var l;var m;var t;var q;s=c._;a:while(true){n=c._;d=true;b:while(d===true){d=false;g=true;c:while(g===true){g=false;o=c._;h=true;d:while(h===true){h=false;c.C=c._;if(!k(c,1,'ß')){break d}c.B=c._;if(!b(c,'ss')){return false}break c}t=c._=o;if(t>=c.A){break b}c._++}continue a}c._=n;break a}c._=s;b:while(true){p=c._;i=true;d:while(i===true){i=false;e:while(true){e=c._;j=true;a:while(j===true){j=false;if(!f(c,a.g_v,97,252)){break a}c.C=c._;l=true;f:while(l===true){l=false;r=c._;m=true;c:while(m===true){m=false;if(!k(c,1,'u')){break c}c.B=c._;if(!f(c,a.g_v,97,252)){break c}if(!b(c,'U')){return false}break f}c._=r;if(!k(c,1,'y')){break a}c.B=c._;if(!f(c,a.g_v,97,252)){break a}if(!b(c,'Y')){return false}}c._=e;break e}q=c._=e;if(q>=c.A){break d}c._++}continue b}c._=p;break b}return true};a.prototype.S=function(){var j;var b;var d;var e;var c;var g;var h;var i;var l;var k;this.I_p1=i=this.A;this.I_p2=i;j=l=this._;b=l+3|0;if(0>b||b>i){return false}k=this._=b;this.I_x=k;this._=j;a:while(true){d=true;b:while(d===true){d=false;if(!f(this,a.g_v,97,252)){break b}break a}if(this._>=this.A){return false}this._++}a:while(true){e=true;b:while(e===true){e=false;if(!n(this,a.g_v,97,252)){break b}break a}if(this._>=this.A){return false}this._++}this.I_p1=this._;c=true;a:while(c===true){c=false;if(!(this.I_p1<this.I_x)){break a}this.I_p1=this.I_x}a:while(true){g=true;b:while(g===true){g=false;if(!f(this,a.g_v,97,252)){break b}break a}if(this._>=this.A){return false}this._++}a:while(true){h=true;b:while(h===true){h=false;if(!n(this,a.g_v,97,252)){break b}break a}if(this._>=this.A){return false}this._++}this.I_p2=this._;return true};a.prototype.r_mark_regions=a.prototype.S;function F(b){var k;var c;var e;var g;var d;var h;var i;var j;var m;var l;b.I_p1=j=b.A;b.I_p2=j;k=m=b._;c=m+3|0;if(0>c||c>j){return false}l=b._=c;b.I_x=l;b._=k;a:while(true){e=true;b:while(e===true){e=false;if(!f(b,a.g_v,97,252)){break b}break a}if(b._>=b.A){return false}b._++}a:while(true){g=true;b:while(g===true){g=false;if(!n(b,a.g_v,97,252)){break b}break a}if(b._>=b.A){return false}b._++}b.I_p1=b._;d=true;a:while(d===true){d=false;if(!(b.I_p1<b.I_x)){break a}b.I_p1=b.I_x}a:while(true){h=true;b:while(h===true){h=false;if(!f(b,a.g_v,97,252)){break b}break a}if(b._>=b.A){return false}b._++}a:while(true){i=true;b:while(i===true){i=false;if(!n(b,a.g_v,97,252)){break b}break a}if(b._>=b.A){return false}b._++}b.I_p2=b._;return true};a.prototype.T=function(){var c;var e;var d;b:while(true){e=this._;d=true;a:while(d===true){d=false;this.C=this._;c=p(this,a.a_0,6);if(c===0){break a}this.B=this._;switch(c){case 0:break a;case 1:if(!b(this,'y')){return false}break;case 2:if(!b(this,'u')){return false}break;case 3:if(!b(this,'a')){return false}break;case 4:if(!b(this,'o')){return false}break;case 5:if(!b(this,'u')){return false}break;case 6:if(this._>=this.A){break a}this._++;break}continue b}this._=e;break b}return true};a.prototype.r_postlude=a.prototype.T;function E(c){var d;var f;var e;b:while(true){f=c._;e=true;a:while(e===true){e=false;c.C=c._;d=p(c,a.a_0,6);if(d===0){break a}c.B=c._;switch(d){case 0:break a;case 1:if(!b(c,'y')){return false}break;case 2:if(!b(c,'u')){return false}break;case 3:if(!b(c,'a')){return false}break;case 4:if(!b(c,'o')){return false}break;case 5:if(!b(c,'u')){return false}break;case 6:if(c._>=c.A){break a}c._++;break}continue b}c._=f;break b}return true};a.prototype.Q=function(){return!(this.I_p1<=this._)?false:true};a.prototype.r_R1=a.prototype.Q;a.prototype.R=function(){return!(this.I_p2<=this._)?false:true};a.prototype.r_R2=a.prototype.R;a.prototype.V=function(){var c;var z;var n;var x;var y;var f;var A;var B;var p;var w;var g;var j;var k;var l;var e;var o;var i;var q;var r;var s;var t;var u;var v;var D;var E;var F;var G;var H;var I;var J;var K;var L;var M;var C;z=this.A-this._;j=true;a:while(j===true){j=false;this.B=this._;c=h(this,a.a_1,7);if(c===0){break a}this.C=D=this._;if(!(!(this.I_p1<=D)?false:true)){break a}switch(c){case 0:break a;case 1:if(!b(this,'')){return false}break;case 2:if(!b(this,'')){return false}n=this.A-this._;k=true;b:while(k===true){k=false;this.B=this._;if(!d(this,1,'s')){this._=this.A-n;break b}this.C=this._;if(!d(this,3,'nis')){this._=this.A-n;break b}if(!b(this,'')){return false}}break;case 3:if(!m(this,a.g_s_ending,98,116)){break a}if(!b(this,'')){return false}break}}G=this._=(F=this.A)-z;x=F-G;l=true;a:while(l===true){l=false;this.B=this._;c=h(this,a.a_2,4);if(c===0){break a}this.C=E=this._;if(!(!(this.I_p1<=E)?false:true)){break a}switch(c){case 0:break a;case 1:if(!b(this,'')){return false}break;case 2:if(!m(this,a.g_st_ending,98,116)){break a}e=this._-3|0;if(this.E>e||e>this.A){break a}this._=e;if(!b(this,'')){return false}break}}C=this._=(M=this.A)-x;y=M-C;o=true;a:while(o===true){o=false;this.B=this._;c=h(this,a.a_4,8);if(c===0){break a}this.C=H=this._;if(!(!(this.I_p2<=H)?false:true)){break a}switch(c){case 0:break a;case 1:if(!b(this,'')){return false}f=this.A-this._;i=true;b:while(i===true){i=false;this.B=this._;if(!d(this,2,'ig')){this._=this.A-f;break b}this.C=I=this._;A=this.A-I;q=true;c:while(q===true){q=false;if(!d(this,1,'e')){break c}this._=this.A-f;break b}J=this._=this.A-A;if(!(!(this.I_p2<=J)?false:true)){this._=this.A-f;break b}if(!b(this,'')){return false}}break;case 2:B=this.A-this._;r=true;b:while(r===true){r=false;if(!d(this,1,'e')){break b}break a}this._=this.A-B;if(!b(this,'')){return false}break;case 3:if(!b(this,'')){return false}p=this.A-this._;s=true;b:while(s===true){s=false;this.B=this._;t=true;c:while(t===true){t=false;w=this.A-this._;u=true;d:while(u===true){u=false;if(!d(this,2,'er')){break d}break c}this._=this.A-w;if(!d(this,2,'en')){this._=this.A-p;break b}}this.C=K=this._;if(!(!(this.I_p1<=K)?false:true)){this._=this.A-p;break b}if(!b(this,'')){return false}}break;case 4:if(!b(this,'')){return false}g=this.A-this._;v=true;b:while(v===true){v=false;this.B=this._;c=h(this,a.a_3,2);if(c===0){this._=this.A-g;break b}this.C=L=this._;if(!(!(this.I_p2<=L)?false:true)){this._=this.A-g;break b}switch(c){case 0:this._=this.A-g;break b;case 1:if(!b(this,'')){return false}break}}break}}this._=this.A-y;return true};a.prototype.r_standard_suffix=a.prototype.V;function A(c){var e;var A;var j;var y;var z;var g;var B;var C;var q;var x;var i;var k;var l;var n;var f;var p;var o;var r;var s;var t;var u;var v;var w;var E;var F;var G;var H;var I;var J;var K;var L;var M;var N;var D;A=c.A-c._;k=true;a:while(k===true){k=false;c.B=c._;e=h(c,a.a_1,7);if(e===0){break a}c.C=E=c._;if(!(!(c.I_p1<=E)?false:true)){break a}switch(e){case 0:break a;case 1:if(!b(c,'')){return false}break;case 2:if(!b(c,'')){return false}j=c.A-c._;l=true;b:while(l===true){l=false;c.B=c._;if(!d(c,1,'s')){c._=c.A-j;break b}c.C=c._;if(!d(c,3,'nis')){c._=c.A-j;break b}if(!b(c,'')){return false}}break;case 3:if(!m(c,a.g_s_ending,98,116)){break a}if(!b(c,'')){return false}break}}H=c._=(G=c.A)-A;y=G-H;n=true;a:while(n===true){n=false;c.B=c._;e=h(c,a.a_2,4);if(e===0){break a}c.C=F=c._;if(!(!(c.I_p1<=F)?false:true)){break a}switch(e){case 0:break a;case 1:if(!b(c,'')){return false}break;case 2:if(!m(c,a.g_st_ending,98,116)){break a}f=c._-3|0;if(c.E>f||f>c.A){break a}c._=f;if(!b(c,'')){return false}break}}D=c._=(N=c.A)-y;z=N-D;p=true;a:while(p===true){p=false;c.B=c._;e=h(c,a.a_4,8);if(e===0){break a}c.C=I=c._;if(!(!(c.I_p2<=I)?false:true)){break a}switch(e){case 0:break a;case 1:if(!b(c,'')){return false}g=c.A-c._;o=true;b:while(o===true){o=false;c.B=c._;if(!d(c,2,'ig')){c._=c.A-g;break b}c.C=J=c._;B=c.A-J;r=true;c:while(r===true){r=false;if(!d(c,1,'e')){break c}c._=c.A-g;break b}K=c._=c.A-B;if(!(!(c.I_p2<=K)?false:true)){c._=c.A-g;break b}if(!b(c,'')){return false}}break;case 2:C=c.A-c._;s=true;b:while(s===true){s=false;if(!d(c,1,'e')){break b}break a}c._=c.A-C;if(!b(c,'')){return false}break;case 3:if(!b(c,'')){return false}q=c.A-c._;t=true;b:while(t===true){t=false;c.B=c._;u=true;c:while(u===true){u=false;x=c.A-c._;v=true;d:while(v===true){v=false;if(!d(c,2,'er')){break d}break c}c._=c.A-x;if(!d(c,2,'en')){c._=c.A-q;break b}}c.C=L=c._;if(!(!(c.I_p1<=L)?false:true)){c._=c.A-q;break b}if(!b(c,'')){return false}}break;case 4:if(!b(c,'')){return false}i=c.A-c._;w=true;b:while(w===true){w=false;c.B=c._;e=h(c,a.a_3,2);if(e===0){c._=c.A-i;break b}c.C=M=c._;if(!(!(c.I_p2<=M)?false:true)){c._=c.A-i;break b}switch(e){case 0:c._=c.A-i;break b;case 1:if(!b(c,'')){return false}break}}break}}c._=c.A-z;return true};a.prototype.J=function(){var f;var g;var h;var b;var a;var c;var d;var i;var j;var e;f=this._;b=true;a:while(b===true){b=false;if(!G(this)){break a}}i=this._=f;g=i;a=true;a:while(a===true){a=false;if(!F(this)){break a}}j=this._=g;this.E=j;this._=this.A;c=true;a:while(c===true){c=false;if(!A(this)){break a}}e=this._=this.E;h=e;d=true;a:while(d===true){d=false;if(!E(this)){break a}}this._=h;return true};a.prototype.stem=a.prototype.J;a.prototype.N=function(b){return b instanceof a};a.prototype.equals=a.prototype.N;a.prototype.O=function(){var c;var a;var b;var d;c='GermanStemmer';a=0;for(b=0;b<c.length;b++){d=c.charCodeAt(b);a=(a<<5)-a+d;a=a&a}return a|0};a.prototype.hashCode=a.prototype.O;a.serialVersionUID=1;g(a,'methodObject',function(){return new a});g(a,'a_0',function(){return[new c('',-1,6),new c('U',0,2),new c('Y',0,1),new c('ä',0,3),new c('ö',0,4),new c('ü',0,5)]});g(a,'a_1',function(){return[new c('e',-1,2),new c('em',-1,1),new c('en',-1,2),new c('ern',-1,1),new c('er',-1,1),new c('s',-1,3),new c('es',5,2)]});g(a,'a_2',function(){return[new c('en',-1,1),new c('er',-1,1),new c('st',-1,2),new c('est',2,1)]});g(a,'a_3',function(){return[new c('ig',-1,1),new c('lich',-1,1)]});g(a,'a_4',function(){return[new c('end',-1,1),new c('ig',-1,2),new c('ung',-1,1),new c('lich',-1,3),new c('isch',-1,2),new c('ik',-1,2),new c('heit',-1,3),new c('keit',-1,4)]});g(a,'g_v',function(){return[17,65,16,1,0,0,0,0,0,0,0,0,0,0,0,0,8,0,32,8]});g(a,'g_s_ending',function(){return[117,30,5]});g(a,'g_st_ending',function(){return[117,30,4]});var q={'src/stemmer.jsx':{Stemmer:o},'src/german-stemmer.jsx':{GermanStemmer:a}}}(JSX))
var Stemmer = JSX.require("src/german-stemmer.jsx").GermanStemmer;



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
    var stopwords = ["aber","alle","allem","allen","aller","alles","als","also","am","an","ander","andere","anderem","anderen","anderer","anderes","anderm","andern","anderr","anders","auch","auf","aus","bei","bin","bis","bist","da","damit","dann","das","dasselbe","dazu","da\u00df","dein","deine","deinem","deinen","deiner","deines","dem","demselben","den","denn","denselben","der","derer","derselbe","derselben","des","desselben","dessen","dich","die","dies","diese","dieselbe","dieselben","diesem","diesen","dieser","dieses","dir","doch","dort","du","durch","ein","eine","einem","einen","einer","eines","einig","einige","einigem","einigen","einiger","einiges","einmal","er","es","etwas","euch","euer","eure","eurem","euren","eurer","eures","f\u00fcr","gegen","gewesen","hab","habe","haben","hat","hatte","hatten","hier","hin","hinter","ich","ihm","ihn","ihnen","ihr","ihre","ihrem","ihren","ihrer","ihres","im","in","indem","ins","ist","jede","jedem","jeden","jeder","jedes","jene","jenem","jenen","jener","jenes","jetzt","kann","kein","keine","keinem","keinen","keiner","keines","k\u00f6nnen","k\u00f6nnte","machen","man","manche","manchem","manchen","mancher","manches","mein","meine","meinem","meinen","meiner","meines","mich","mir","mit","muss","musste","nach","nicht","nichts","noch","nun","nur","ob","oder","ohne","sehr","sein","seine","seinem","seinen","seiner","seines","selbst","sich","sie","sind","so","solche","solchem","solchen","solcher","solches","soll","sollte","sondern","sonst","um","und","uns","unse","unsem","unsen","unser","unses","unter","viel","vom","von","vor","war","waren","warst","was","weg","weil","weiter","welche","welchem","welchen","welcher","welches","wenn","werde","werden","wie","wieder","will","wir","wird","wirst","wo","wollen","wollte","w\u00e4hrend","w\u00fcrde","w\u00fcrden","zu","zum","zur","zwar","zwischen","\u00fcber"];

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