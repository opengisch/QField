/*
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
*/

document.addEventListener("DOMContentLoaded", function() {
  // customer success stories
  (() => {
    const elCustomers = document.querySelector('#customers');
    elCustomers.querySelectorAll('.customer-logos img').forEach((elImg) => {
      elImg.addEventListener('click', () => {
        elCustomers.querySelectorAll(`*[title="${elImg.title}"]`).forEach((el) => {
          [...el.parentElement.children].forEach((elSibling) => elSibling.classList.remove('customer-active'));
          el.classList.add('customer-active');
        });
      });
    });
  })();
});
