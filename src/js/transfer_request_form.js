/**
 * Created by Scott on 23/02/14.
 */

(function ($) {
    Drupal.behaviors.transfer_request = {
        attach: function (context, settings) {

            //$('table tr td:first-child, thead tr th:first-child').remove(); // Remove the checkbox in table.

            $('.approval-fieldset', context).once(function (context) {
                $(this).hide(); // Hide the forms on page load.
            });

            // Toggle fieldsets when user clicks show Details or the Cancel button
            $(".show-details, .cancel-btn-val", context).click(function () {

                table_num = $(this).attr('value');
                selFieldset = 'fieldset#transfer-order-' + table_num;

                var selectdiv = $(selFieldset);
                var elem = selectdiv.slideToggle(600);
                $('.approval-fieldset').not(elem).hide();

            }); // End click event
        }
    };

    Drupal.behaviors.transfer_request_print = {
        attach: function (context, settings) {

            // When user clicks Print on the print
            // pages we open the a printer window
            $("#print-this-page", context).click(function() {

                $('div#marble-top').css({
                    'box-shadow': 'none'
                });

                $('div.alert-block').remove();

                // Now wait for sass to change on page before loading and sending to printer
                setTimeout(function(){
                    // Now we can print page
                    window.print();
                }, 400);
            });
        }
    };


})(jQuery);
