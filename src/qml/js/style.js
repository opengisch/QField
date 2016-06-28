function getThemeIcon(name) {

  var ppi = dp / 0.00768443 * 25.4;

  var dir = ["mdpi","hdpi","xhdpi","xxhdpi", "xxxhdpi"];

  var ppiRange
  if ( ppi >= 360 )
    ppi = 4;
   else if ( ppi >= 270 )
    ppi = 3;
   else if ( ppi >= 180 )
    ppi = 2;
   else if ( ppi >= 135 )
    ppi = 1;
   else
    ppi = 0;

  var theme = 'qfield';

  console.info( ppi + ": " + dir[ppi] )

  var path = '/themes/' + theme + '/' + dir[ppi] + '/' + name + '.png';
  return path;
}
