$(window).scroll(function() {
  if ($(document).scrollTop() > 50) {
    $('div.navbar').addClass('shrink');
    $('.navbar-brand>span').addClass('unobtrusive');
    $('.shadow-background').addClass('unobtrusive');
    $('#qfield-logo').addClass('unobtrusive');
  } else {
    $('div.navbar').removeClass('shrink');
    $('.navbar-brand>span').removeClass('unobtrusive');
    $('.shadow-background').removeClass('unobtrusive');
    $('#qfield-logo').removeClass('unobtrusive');
  }
});
