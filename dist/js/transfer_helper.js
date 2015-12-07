(function($, Drupal) {

  Drupal.ajax.prototype.commands.transfer_help = function(ajax, response, status) {

    var id;
    id = response.id;
    console.log('shit');
    var removeBtn = $('remove-field-1');
    var unitIdSelect = $('.unit-id-select');

    //removeBtn.click(function() {
    //  $('#rig-location-data-' + id).remove().parent();
    //});

    //$('#rig-location-data-' + id).remove();
    //console.log('#rig-location-data-' + id);

    //$('#unit-id-' + id).change(function() {
    //  $('#from-field-' + id).val('');
    //});

  }
})(jQuery, Drupal);

//(function($) {
//
//    $(document).ready(function() {
//
//      var removeBtn = $('remove-field-1');
//      var unitIdSelect = $('.unit-id-select')
//
//      removeBtn.click(function() {
//        $('#rig-location-data-1').remove();
//      });
//
//    });
//
//})(jQuery);