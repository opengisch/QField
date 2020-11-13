/*
 * language_data.js
 * ~~~~~~~~~~~~~~~~
 *
 * This script contains the language-specific data used by searchtools.js,
 * namely the list of stopwords, stemmer, scorer and splitter.
 *
 * :copyright: Copyright 2007-2019 by the Sphinx team, see AUTHORS.
 * :license: BSD, see LICENSE for details.
 *
 */

var stopwords = ["alla","allt","att","av","blev","bli","blir","blivit","de","dem","den","denna","deras","dess","dessa","det","detta","dig","din","dina","ditt","du","d\u00e4r","d\u00e5","efter","ej","eller","en","er","era","ert","ett","fr\u00e5n","f\u00f6r","ha","hade","han","hans","har","henne","hennes","hon","honom","hur","h\u00e4r","i","icke","ingen","inom","inte","jag","ju","kan","kunde","man","med","mellan","men","mig","min","mina","mitt","mot","mycket","ni","nu","n\u00e4r","n\u00e5gon","n\u00e5got","n\u00e5gra","och","om","oss","p\u00e5","samma","sedan","sig","sin","sina","sitta","sj\u00e4lv","skulle","som","s\u00e5","s\u00e5dan","s\u00e5dana","s\u00e5dant","till","under","upp","ut","utan","vad","var","vara","varf\u00f6r","varit","varje","vars","vart","vem","vi","vid","vilka","vilkas","vilken","vilket","v\u00e5r","v\u00e5ra","v\u00e5rt","\u00e4n","\u00e4r","\u00e5t","\u00f6ver"];


/* Non-minified version JS is _stemmer.js if file is provided */ 
var JSX={};(function(e){function i(b,e){var a=function(){};a.prototype=e.prototype;var c=new a;for(var d in b){b[d].prototype=c}}function G(c,b){for(var a in b.prototype)if(b.prototype.hasOwnProperty(a))c.prototype[a]=b.prototype[a]}function h(a,b,d){function c(a,b,c){delete a[b];a[b]=c;return c}Object.defineProperty(a,b,{get:function(){return c(a,b,d())},set:function(d){c(a,b,d)},enumerable:true,configurable:true})}function F(a,b,c){return a[b]=a[b]/c|0}var t=parseInt;var u=parseFloat;function E(a){return a!==a}var x=isFinite;var y=encodeURIComponent;var z=decodeURIComponent;var B=encodeURI;var C=decodeURI;var o=Object.prototype.toString;var p=Object.prototype.hasOwnProperty;function f(){}e.require=function(b){var a=n[b];return a!==undefined?a:null};e.profilerIsRunning=function(){return f.getResults!=null};e.getProfileResults=function(){return(f.getResults||function(){return{}})()};e.postProfileResults=function(a,b){if(f.postResults==null)throw new Error('profiler has not been turned on');return f.postResults(a,b)};e.resetProfileResults=function(){if(f.resetResults==null)throw new Error('profiler has not been turned on');return f.resetResults()};e.DEBUG=false;function r(){};i([r],Error);function a(a,b,c){this.G=a.length;this.R=a;this.U=b;this.J=c;this.I=null;this.V=null};i([a],Object);function j(){};i([j],Object);function d(){var a;var b;var c;this.F={};a=this.C='';b=this._=0;c=this.B=a.length;this.A=0;this.D=b;this.E=c};i([d],j);function v(a,b){a.C=b.C;a._=b._;a.B=b.B;a.A=b.A;a.D=b.D;a.E=b.E};function k(b,d,c,e){var a;if(b._>=b.B){return false}a=b.C.charCodeAt(b._);if(a>e||a<c){return false}a-=c;if((d[a>>>3]&1<<(a&7))===0){return false}b._++;return true};function l(b,d,c,e){var a;if(b._<=b.A){return false}a=b.C.charCodeAt(b._-1);if(a>e||a<c){return false}a-=c;if((d[a>>>3]&1<<(a&7))===0){return false}b._--;return true};function m(a,d,c,e){var b;if(a._>=a.B){return false}b=a.C.charCodeAt(a._);if(b>e||b<c){a._++;return true}b-=c;if((d[b>>>3]&1<<(b&7))===0){a._++;return true}return false};function g(d,m,p){var b;var g;var e;var n;var f;var k;var l;var i;var h;var c;var a;var j;var o;b=0;g=p;e=d._;n=d.A;f=0;k=0;l=false;while(true){i=b+(g-b>>1);h=0;c=f<k?f:k;a=m[i];for(j=a.G-1-c;j>=0;j--){if(e-c===n){h=-1;break}h=d.C.charCodeAt(e-1-c)-a.R.charCodeAt(j);if(h!==0){break}c++}if(h<0){g=i;k=c}else{b=i;f=c}if(g-b<=1){if(b>0){break}if(g===b){break}if(l){break}l=true}}while(true){a=m[b];if(f>=a.G){d._=e-a.G|0;if(a.I==null){return a.J}o=a.I(d);d._=e-a.G|0;if(o){return a.J}}b=a.U;if(b<0){return 0}}return-1};function A(a,b,d,e){var c;c=e.length-(d-b);a.C=a.C.slice(0,b)+e+a.C.slice(d);a.B+=c|0;if(a._>=d){a._+=c|0}else if(a._>b){a._=b}return c|0};function c(a,f){var b;var c;var d;var e;b=false;if((c=a.D)<0||c>(d=a.E)||d>(e=a.B)||e>a.C.length?false:true){A(a,a.D,a.E,f);b=true}return b};d.prototype.H=function(){return false};d.prototype.S=function(b){var a;var c;var d;var e;a=this.F['.'+b];if(a==null){c=this.C=b;d=this._=0;e=this.B=c.length;this.A=0;this.D=d;this.E=e;this.H();a=this.C;this.F['.'+b]=a}return a};d.prototype.stemWord=d.prototype.S;d.prototype.T=function(e){var d;var b;var c;var a;var f;var g;var h;d=[];for(b=0;b<e.length;b++){c=e[b];a=this.F['.'+c];if(a==null){f=this.C=c;g=this._=0;h=this.B=f.length;this.A=0;this.D=g;this.E=h;this.H();a=this.C;this.F['.'+c]=a}d.push(a)}return d};d.prototype.stemWords=d.prototype.T;function b(){d.call(this);this.I_x=0;this.I_p1=0};i([b],d);b.prototype.K=function(a){this.I_x=a.I_x;this.I_p1=a.I_p1;v(this,a)};b.prototype.copy_from=b.prototype.K;b.prototype.P=function(){var g;var d;var a;var e;var c;var f;var i;var j;var l;var h;this.I_p1=j=this.B;g=i=this._;a=i+3|0;if(0>a||a>j){return false}h=this._=a;this.I_x=h;this._=g;a:while(true){d=this._;e=true;b:while(e===true){e=false;if(!k(this,b.g_v,97,246)){break b}this._=d;break a}l=this._=d;if(l>=this.B){return false}this._++}a:while(true){c=true;b:while(c===true){c=false;if(!m(this,b.g_v,97,246)){break b}break a}if(this._>=this.B){return false}this._++}this.I_p1=this._;f=true;a:while(f===true){f=false;if(!(this.I_p1<this.I_x)){break a}this.I_p1=this.I_x}return true};b.prototype.r_mark_regions=b.prototype.P;function D(a){var h;var e;var c;var f;var d;var g;var j;var l;var n;var i;a.I_p1=l=a.B;h=j=a._;c=j+3|0;if(0>c||c>l){return false}i=a._=c;a.I_x=i;a._=h;a:while(true){e=a._;f=true;b:while(f===true){f=false;if(!k(a,b.g_v,97,246)){break b}a._=e;break a}n=a._=e;if(n>=a.B){return false}a._++}a:while(true){d=true;b:while(d===true){d=false;if(!m(a,b.g_v,97,246)){break b}break a}if(a._>=a.B){return false}a._++}a.I_p1=a._;g=true;a:while(g===true){g=false;if(!(a.I_p1<a.I_x)){break a}a.I_p1=a.I_x}return true};b.prototype.O=function(){var a;var e;var d;var f;var h;var i;e=this.B-(f=this._);if(f<this.I_p1){return false}h=this._=this.I_p1;d=this.A;this.A=h;i=this._=this.B-e;this.E=i;a=g(this,b.a_0,37);if(a===0){this.A=d;return false}this.D=this._;this.A=d;switch(a){case 0:return false;case 1:if(!c(this,'')){return false}break;case 2:if(!l(this,b.g_s_ending,98,121)){return false}if(!c(this,'')){return false}break}return true};b.prototype.r_main_suffix=b.prototype.O;function w(a){var d;var f;var e;var h;var i;var j;f=a.B-(h=a._);if(h<a.I_p1){return false}i=a._=a.I_p1;e=a.A;a.A=i;j=a._=a.B-f;a.E=j;d=g(a,b.a_0,37);if(d===0){a.A=e;return false}a.D=a._;a.A=e;switch(d){case 0:return false;case 1:if(!c(a,'')){return false}break;case 2:if(!l(a,b.g_s_ending,98,121)){return false}if(!c(a,'')){return false}break}return true};b.prototype.N=function(){var e;var a;var f;var h;var i;var j;var k;var d;e=this.B-(h=this._);if(h<this.I_p1){return false}i=this._=this.I_p1;a=this.A;this.A=i;k=this._=(j=this.B)-e;f=j-k;if(g(this,b.a_1,7)===0){this.A=a;return false}d=this._=this.B-f;this.E=d;if(d<=this.A){this.A=a;return false}this._--;this.D=this._;if(!c(this,'')){return false}this.A=a;return true};b.prototype.r_consonant_pair=b.prototype.N;function s(a){var f;var d;var h;var i;var j;var k;var l;var e;f=a.B-(i=a._);if(i<a.I_p1){return false}j=a._=a.I_p1;d=a.A;a.A=j;l=a._=(k=a.B)-f;h=k-l;if(g(a,b.a_1,7)===0){a.A=d;return false}e=a._=a.B-h;a.E=e;if(e<=a.A){a.A=d;return false}a._--;a.D=a._;if(!c(a,'')){return false}a.A=d;return true};b.prototype.Q=function(){var d;var e;var a;var f;var h;var i;e=this.B-(f=this._);if(f<this.I_p1){return false}h=this._=this.I_p1;a=this.A;this.A=h;i=this._=this.B-e;this.E=i;d=g(this,b.a_2,5);if(d===0){this.A=a;return false}this.D=this._;switch(d){case 0:this.A=a;return false;case 1:if(!c(this,'')){return false}break;case 2:if(!c(this,'lös')){return false}break;case 3:if(!c(this,'full')){return false}break}this.A=a;return true};b.prototype.r_other_suffix=b.prototype.Q;function q(a){var e;var f;var d;var h;var i;var j;f=a.B-(h=a._);if(h<a.I_p1){return false}i=a._=a.I_p1;d=a.A;a.A=i;j=a._=a.B-f;a.E=j;e=g(a,b.a_2,5);if(e===0){a.A=d;return false}a.D=a._;switch(e){case 0:a.A=d;return false;case 1:if(!c(a,'')){return false}break;case 2:if(!c(a,'lös')){return false}break;case 3:if(!c(a,'full')){return false}break}a.A=d;return true};b.prototype.H=function(){var g;var f;var h;var b;var c;var a;var d;var i;var j;var k;var l;var e;g=this._;b=true;a:while(b===true){b=false;if(!D(this)){break a}}i=this._=g;this.A=i;k=this._=j=this.B;f=j-k;c=true;a:while(c===true){c=false;if(!w(this)){break a}}e=this._=(l=this.B)-f;h=l-e;a=true;a:while(a===true){a=false;if(!s(this)){break a}}this._=this.B-h;d=true;a:while(d===true){d=false;if(!q(this)){break a}}this._=this.A;return true};b.prototype.stem=b.prototype.H;b.prototype.L=function(a){return a instanceof b};b.prototype.equals=b.prototype.L;b.prototype.M=function(){var c;var a;var b;var d;c='SwedishStemmer';a=0;for(b=0;b<c.length;b++){d=c.charCodeAt(b);a=(a<<5)-a+d;a=a&a}return a|0};b.prototype.hashCode=b.prototype.M;b.serialVersionUID=1;h(b,'methodObject',function(){return new b});h(b,'a_0',function(){return[new a('a',-1,1),new a('arna',0,1),new a('erna',0,1),new a('heterna',2,1),new a('orna',0,1),new a('ad',-1,1),new a('e',-1,1),new a('ade',6,1),new a('ande',6,1),new a('arne',6,1),new a('are',6,1),new a('aste',6,1),new a('en',-1,1),new a('anden',12,1),new a('aren',12,1),new a('heten',12,1),new a('ern',-1,1),new a('ar',-1,1),new a('er',-1,1),new a('heter',18,1),new a('or',-1,1),new a('s',-1,2),new a('as',21,1),new a('arnas',22,1),new a('ernas',22,1),new a('ornas',22,1),new a('es',21,1),new a('ades',26,1),new a('andes',26,1),new a('ens',21,1),new a('arens',29,1),new a('hetens',29,1),new a('erns',21,1),new a('at',-1,1),new a('andet',-1,1),new a('het',-1,1),new a('ast',-1,1)]});h(b,'a_1',function(){return[new a('dd',-1,-1),new a('gd',-1,-1),new a('nn',-1,-1),new a('dt',-1,-1),new a('gt',-1,-1),new a('kt',-1,-1),new a('tt',-1,-1)]});h(b,'a_2',function(){return[new a('ig',-1,1),new a('lig',0,1),new a('els',-1,1),new a('fullt',-1,3),new a('löst',-1,2)]});h(b,'g_v',function(){return[17,65,16,1,0,0,0,0,0,0,0,0,0,0,0,0,24,0,32]});h(b,'g_s_ending',function(){return[119,127,149]});var n={'src/stemmer.jsx':{Stemmer:j},'src/swedish-stemmer.jsx':{SwedishStemmer:b}}}(JSX))
var Stemmer = JSX.require("src/swedish-stemmer.jsx").SwedishStemmer;





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


