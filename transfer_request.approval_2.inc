<?php
require_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'transfer_request') . '/includes/class.transfer_request.inc.php');
drupal_add_css(drupal_get_path('module', 'transfer_request') . '/dist/css/transfer_request_form.css');

/**
 * Show form once callback has fired.
 *
 * @return mixed
 */
function transfer_request_approval_page() {
  global $user;
  $build = array();

  // Get our db object
  $transfer_obj = new TransferRequest();
  $trans_data = $transfer_obj->get_transfer_approval_data();

  $build['help_text'] = array(
    '#type' => 'markup',
    '#markup' => "<div class='repair-message'><p>Click the 'Show Details' link
          to edit Transfer Request Order and then click 'Update' or 'Approve Order'.</p></div>",
  );

  // Call our forms
  foreach ($trans_data as $key => $val) {
    $build[] = drupal_get_form('transfer_request_approval_form_' . $val['transfer_num'], $val);
  }

  return $build;
}

/**
 * Transfer_request_approval_form
 */
function transfer_request_approval_form($form, &$form_state, $args) {
//  dsm($form_state);
  global $user;
  $_user = $user->name;
  $match = array();
  $is_unit_trigger = array();

  $transfer_obj_2 = new TransferRequest();
//  $transfer_num = $transfer_obj->transfer_value();
  $unit_id_select = $transfer_obj_2->unit_id_value();

  // Grab passed in arguments
  $transfer_data = $args;

  $transfer_num = !empty($form_state['values']['location' . $transfer_data['transfer_num']]['transfer_num'])
    ? $form_state['values']['location' . $transfer_data['transfer_num']]['transfer_num'] : $transfer_data['transfer_num'];
  $submitted_by = $transfer_data['submitted_by'];
  $date_submitted = $transfer_data['date_submitted'];
  $operator = $transfer_data['operator'];
  $to_rig = $transfer_data['to_rig'];
  $to_location = $transfer_data['to_location'];
  $transport_company = $transfer_data['transport_company'];
  $driver_name = $transfer_data['driver_name'];
  $receiver = $transfer_data['receiver'];
  $phone_num = $transfer_data['phone_num'];
  $email = $transfer_data['email'];
  $shipping_directions = $transfer_data['shipping_directions'];
  $shipping_notes = $transfer_data['shipping_notes'];
//  $third_party = $transfer_data['third_party_rental'];

  $transfer_items = unserialize($transfer_data['transfer_items']);

  // Create header array for table
  $header = array(
    'transfer_num' => t('Transfer#'),
    'more_details' => t('Details'),
    'date_submitted' => t('Date'),
    'receiver' => t('Receiver'),
    'submitted_by' => t('Submitted by'),
  );

  // Link for modal forms
  $links['pop_form'] = '<span value="' . $transfer_num . '" class="show-details">Show Details</span>';

  $rows = array();
  // Build the rows and add data
  $rows[$transfer_num] = array(
    'transfer_num' => $transfer_num,
    'more_details' => $links['pop_form'],
    'date_submitted' => $date_submitted,
    'receiver' => $receiver,
    'submitted_by' => $submitted_by,
  );


  // Construct the table
  $form['approval_table_' . $transfer_num] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#js_select' => FALSE,
    '#empty' => t('No users found'),
    '#multiple' => TRUE,
  );


  $form['#tree'] = TRUE;

  $form['field_instructions'] = array(
    '#type' => 'markup',
    '#markup' => '<p><strong class="field_instructions">Fields</strong> marked with a
      <strong>red asterick = " <span class="field_instructions">*</span> "</strong> are required.</p>',
  );

  # Contact information field.
  $form['contact_info' . $transfer_num] = array(
    '#type' => 'fieldset',
    '#title' => t('Contact Information'),
  );
  # Serial number
  $form['contact_info' . $transfer_num]['transfer_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Transfer #'),
    '#value' => $transfer_num,
    '#size' => 11,
    '#disabled' => TRUE,
    '#prefix' => '<div id="transfer-num">',
    '#suffix' => '</div>',
  );
  # Date field
  $form['contact_info' . $transfer_num]['date'] = array(
    '#type' => 'textfield',
    '#title' => t('Date'),
    '#value' => date('M d, Y'),
    '#size' => 30,
    '#disabled' => TRUE,
    '#prefix' => '<div id="date-submitted">',
    '#suffix' => '</div>',
  );
  # Contact Info
  $form['contact_info' . $transfer_num]['submitted_by'] = array(
    '#type' => 'textfield',
    '#title' => t('Submitted by'),
    '#value' => $_user,
    '#disabled' => TRUE,
    '#prefix' => '<div id="submitted-by">',
    '#suffix' => '</div>',
  );


  # Unit ID selector, Rig and From LSD fields
  $form["location" . $transfer_num] = array(
    '#type' => 'fieldset',
    '#title' => t("Originating Location"),
    '#id' => 'ajax-transfer-items-container',
    //    '#attributes' => array('class' => array('row')),
  );

//  $form["location" . $transfer_num]['items'] = array(
//    '#type' => 'container',
//    '#id' => 'ajax-transfer-items-container',
//    '#default_value' => count($transfer_items),
//  );

  // $form_state['location']['items'] to determine the number of textfields to build.
  if (empty($form_state['location' . $transfer_num]['items'])) {
//    $form_state['location' . $transfer_num]['items'][] = count($transfer_items);
    $form_state['location' . $transfer_num]['items'] = $transfer_items;
  }

  for ($i = 0; $i < $form_state['location' . $transfer_num]['items'];  $i++) {
  // Add our Transfer Items to the form array
//  foreach ($form_state['location' . $transfer_num]['items'][$transfer_num] as $key => $value) {
//dsm($value);
    // Grab the word unit form triggering element to use to filter which button was selected
    $triggering_ele = isset($form_state['triggering_element']['#id']) ? $form_state['triggering_element']['#id'] : '';
    preg_match('/(\w)*/', $triggering_ele, $is_unit_trigger); // Get the first word of unit #id

    // Get each triggering element number off end of triggering elements #id
    $trigger = isset($form_state['triggering_element']['#id']) ? $form_state['triggering_element']['#id'] : '';
    preg_match('/(\d+)$/', $trigger, $match);
    $match = array_shift($match);

    if (is_numeric($match) && $is_unit_trigger[0] == 'unit') {

      $nid = isset($form_state['values']["location" . $transfer_num]['items'][$i]["unit_id_select"])
        ? $form_state['values']["location" . $transfer_num]['items'][$i]["unit_id_select"] : '';

      // Grab the data, send nid and item number as params
      $rig_location = $transfer_obj_2->get_rig_location_values($nid, $match);

      // Save nid and any other data for validate and submit function
      $form_state['transfer_data'][$i]['unit_id'][] = $rig_location[$i]->title;

      // Assign data to $form_state
      $form_state['values']['location' . $transfer_num]['items'][$i]['data']['from_field'][] =
        $rig_location[$i]->field_rig_value;

      $form_state['values']['location' . $transfer_num]['items'][$i]['data']['from_lsd'][] =
        $rig_location[$i]->field_location_value;
    }

    $form_state['transfer_values'] = array(
      '#type' => 'value',
      '#value' => '',
    );
//    dsm($form_state['location' . $transfer_num]['items']);
      $form["location" . $transfer_num]['items'][$i]["item_num"] = array(
        '#type' => 'textfield',
        '#title' => 'Item #',
        '#default_value' => $i + 1,
        '#id' => 'item-' . $i,
        '#prefix' => '<div class="transfer-items-num">',
        '#suffix' => '</div>',
      );

      $form["location" . $transfer_num]['items'][$i]["unit_id_select"] = array(
        '#type' => 'select',
        '#title' => t('Unit ID'),
        '#empty_option' => !empty($value['unit_id']) ? $value['unit_id'] : t('- Select Unit ID -'),
        '#options' => $unit_id_select,
        '#id' => 'unit-id-' . $i,
        //    '#required' => TRUE,
        '#prefix' => '<div class="unit-id-select">',
        '#suffix' => '</div>',
        '#ajax' => array(
          'callback' => 'get_location_callback_ajax',
          'wrapper' => 'rig-location-data-' . $i,
          'method' => 'replace',
        ),
      );

      $form["location" . $transfer_num]['items'][$i]['data'] = array(
        '#type' => 'container',
        '#id' => 'rig-location-data-' . $i,
      );


      $form["location" . $transfer_num]['items'][$i]['data']["from_field"] = array(
        '#type' => 'textfield',
        '#title' => t('Rig #'),
        '#default_value' => isset($form_state['values']['location']['items'][$i]['data']['from_field'])
          ? $form_state['values']['location']['items'][$i]['data']['from_field'] : $value['from_field'],
        '#id' => "from-field-" . $i,
        '#disabled' => TRUE,
        //      '#attributes' => array('class' => array('transfer-items')),
        '#prefix' => '<div class="from-field">',
        '#suffix' => '</div>',
      );
      $form["location" . $transfer_num]['items'][$i]['data']["from_lsd"] = array(
        '#type' => 'textfield',
        '#title' => t('Current Location'),
        '#default_value' => isset($form_state['values']['location']['items'][$i]['data']['from_lsd'])
          ? $form_state['values']['location']['items'][$i]['data']['from_lsd'] : $value['from_lsd'],
        '#description' => t('L.S.D.'),
        '#id' => "from-lsd-" . $i,
        '#disabled' => TRUE,
        //      '#attributes' => array('class' => array('transfer-items')),
        '#prefix' => '<div class="from-lsd">',
        '#suffix' => '</div>',
      );
    }
//  }


  // Submit button to add more repairs text fields
  $form['add_repair' . $transfer_num] = array(
    '#type' => 'submit',
    '#value' => 'Add Item',
    '#submit' => array('transfer_order_add_textfield'),
    '#ajax' => array(
      'callback' => 'ajax_transfer_order_callback',
      'wrapper' => 'ajax-transfer-items-container',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#prefix' => '<div class="add">',
    '#suffix' => '</div>',
  );
  // Submit button to remove repairs text fields
  $form['remove_repair' . $transfer_num] = array(
    '#type' => 'submit',
    '#value' => 'Remove Item',
    '#submit' => array('transfer_order_remove_textfield'),
    '#ajax' => array(
      'callback' => 'ajax_transfer_order_callback',
      'wrapper' => 'ajax-transfer-items-container',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#prefix' => '<div class="remove">',
    '#suffix' => '</div>',
  );

  // Destination location field.
  $form['dest_location' . $transfer_num] = array(
    '#type' => 'fieldset',
    '#title' => t('Destination Location'),
    '#prefix' => '<div id="dest-location">',
    '#suffix' => '</div>',
  );
  // To field
  $form['dest_location' . $transfer_num]['to_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Rig #'),
    //      '#required' => TRUE,
    '#prefix' => '<div id="to-field">',
    '#suffix' => '</div>',
  );
  $form['dest_location' . $transfer_num]['operator'] = array(
    '#type' => 'textfield',
    '#title' => t('Operator'),
    //    '#required' => TRUE,
    '#prefix' => '<div id="operator">',
    '#suffix' => '</div>',
  );
  // To LSD
  $form['dest_location' . $transfer_num]['to_lsd'] = array(
    '#type' => 'textfield',
    '#title' => t('Location'),
    '#description' => t('L.S.D.'),
    //      '#required' => TRUE,
    '#prefix' => '<div id="to-lsd">',
    '#suffix' => '</div>',
  );
  // Shipping details
  $form['ship_details' . $transfer_num] = array(
    '#type' => 'fieldset',
    '#title' => t('Shipping Details'),
  );
  $form['ship_details' . $transfer_num]['trans_company'] = array(
    '#type' => 'textfield',
    '#title' => t('Transport Company'),
    '#prefix' => '<div id="trans-company">',
    '#suffix' => '</div>',
  );
  $form['ship_details' . $transfer_num]['drivers_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Drivers Name'),
    '#prefix' => '<div id="drivers-name">',
    '#suffix' => '</div>',
  );
  $form['ship_details' . $transfer_num]['receiver'] = array(
    '#type' => 'textfield',
    '#title' => t('Receiver'),
    //      '#required' => TRUE,
    '#prefix' => '<div id="receiver">',
    '#suffix' => '</div>',
  );
  $form['ship_details' . $transfer_num]['phone_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone Number'),
    '#prefix' => '<div id="phone-num">',
    '#suffix' => '</div>',
  );
  $form['ship_details' . $transfer_num]['receiver_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#prefix' => '<div id="receiver-email">',
    '#suffix' => '</div>',
  );
  $form['ship_details' . $transfer_num]['ship_directions'] = array(
    '#type' => 'textarea',
    '#title' => t('Shipping Directions'),
    '#prefix' => '<div id="ship-directions">',
    '#suffix' => '</div>',
  );
  $form['ship_details' . $transfer_num]['shipping_notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#prefix' => '<div id="shipping-notes">',
    '#suffix' => '</div>',
  );

  $form['third_party_rental' . $transfer_num] = array(
    '#type' => 'checkbox',
    '#title' => t('Applicable Third Party Charges'),
    '#id' => 'third-party-checkbox',
    '#prefix' => '<div id="third-party" class="checkbox">',
    '#suffix' => '</div>',
  );

  // Submit button
  $form['submit_button' . $transfer_num] = array(
    '#type' => 'submit',
    '#value' => t('Submit For Approval'),
    '#id' => 'my-submit-button',
    '#prefix' => '<div id="submit-button">',
    '#suffix' => '</div>',
  );

  return $form;
}

/**
 * Get values for the 'from' and 'from_location' fields
 */
function get_location_callback_ajax($form, &$form_state) {

  $item = array();
  // Get each triggering element number off end of triggering elements #id
  preg_match('/(\d+)$/', $form_state['triggering_element']['#id'], $item);
  // grab first string value
  $item = array_shift($item);

  return $form["location". $item]['items'][$item]['data'];
}


// Ajax function called and used to rebuild form
function ajax_transfer_order_callback($form, &$form_state) {

  $item = array();
  // Get each triggering element number off end of triggering elements #id
  preg_match('/(\d+)$/', $form_state['triggering_element']['#id'], $item);
  // grab first string value
  $item = array_shift($item);

  return $form['location' . $item];
}

/**
 * Submit handler for the "add repair" button.
 *
 * Increments the repairs_req fields and causes a rebuild.
 */
function transfer_order_add_textfield($form, &$form_state) {

  $item = array();
  // Get each triggering element number off end of triggering elements #id
  preg_match('/(\d+)$/', $form_state['triggering_element']['#id'], $item);
  // grab first string value
  $item = array_shift($item);
  dsm($item);

  $form_state['location' . $item]['items']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove repair" button.
 *
 * Decrements the repairs_req fields and causes a form rebuild.
 */
function transfer_order_remove_textfield($form, &$form_state) {

  $item = array();
  // Get each triggering element number off end of triggering elements #id
  preg_match('/(\d+)$/', $form_state['triggering_element']['#id'], $item);
  // grab first string value
  $item = array_shift($item);

  if ($form_state['location' . $item]['items'] > 1) {
    $form_state['location' . $item]['items']--;
  }
  $form_state['rebuild'] = TRUE;
}


/**
 * Validate the form data
 * @param $form
 * @param $form_state
 */
function transfer_request_approval_form_validate($form, &$form_state) {
dsm($form_state);
//  $trans_val = array();
//
//  // If email is supplied then validate it
//  if(!empty($form_state['values']['ship_details']['receiver_email'])) {
//    $mail = $form_state['values']['ship_details']['receiver_email'];
//    // Pass result to $answer variable
//    $answer = valid_email_address($mail);
//
//    if($answer == FALSE) {
//      form_set_error('ship_details][receiver_email',
//        t('Please enter a proper email address!'));
//    }
//  }
//
//  // Organize our values for the submit function
//  for ($t = 0; $t < $form_state['location']['items']; $t++) {
//
//    $nid = $form_state['values']['location']['items'][$t]['unit_id_select'];
//
//    $trans_val[$t]['nid'] = $nid;
//    $trans_val[$t] ['unit_id'] = $form_state['transfer_data'][$t]['unit_id'][0];
//    $trans_val[$t]['from_field'] = $form_state['values']['location']['items'][$t]['data']['from_field'][0];
//    $trans_val[$t]['from_lsd'] = $form_state['values']['location']['items'][$t]['data']['from_lsd'][0];
//  }
////  dsm($trans_val);
//  // Serialize and store our transfer_values back to form_state
//  $form_state['transfer_values'] = serialize($trans_val);

}

/**
 * Submit the data to the database.
 * @param $form
 * @param $form_state
 */
function transfer_request_form_submit($form, &$form_state) {

  $transfer_num = $form_state['values']['contact_info']['transfer_number'];

  $fields = array(
    'transfer_num' => $form_state['values']['contact_info']['transfer_number'],
    'date_submitted' => $form_state['values']['contact_info']['date'],
    'submitted_by' => $form_state['values']['contact_info']['submitted_by'],
    'transport_company' => $form_state['values']['ship_details']['trans_company'],
    'driver_name' => $form_state['values']['ship_details']['drivers_name'],
    'receiver' => $form_state['values']['ship_details']['receiver'],
    'phone_num' => $form_state['values']['ship_details']['phone_num'],
    'email' => $form_state['values']['ship_details']['receiver_email'],
    'shipping_directions' => $form_state['values']['ship_details']['ship_directions'],
    'shipping_notes' => $form_state['values']['ship_details']['shipping_notes'],
    'transfer_items' => $form_state['transfer_values'],
    'to_rig' => $form_state['values']['dest_location']['to_field'],
    'operator' => $form_state['values']['dest_location']['operator'],
    'to_location' => $form_state['values']['dest_location']['to_lsd'],
    'third_party_rental' => $form_state['values']['third_party_rental'],
  );

  $query = db_insert('transfer_request_updated')
    ->fields($fields);

  // Submit to database
//  $query->execute();

  // Start a SESSION to store the user input for the confirmation print page to use.
//  $_SESSION['form_data'] = $fields;


//  global $user;
//  // Get emails that have been set in the transfer request configuration page.
//  $admin_mail = variable_get('transfer_admin_email');
//  $party_mail = variable_get('transfer_party_email');
//  //$cc = variable_get('transfer_cc_email');
//
//  $date = $form_state['values']['contact_info']['date'];
//  // Send email to administrator notifying him of request.
//  $to = $admin_mail;
//  $from = $user->mail;
//  //$header_cc = $cc;
//  $params['transfer_num'] =  $transfer_num;
//  $params['date'] = $date;
//  $params['notes'] = $form_state['values']['ship_details']['shipping_notes'];
//
//  $sent = drupal_mail('transfer_request', 'new_transfer', $to, language_default(), $params, $from, TRUE);
//  // Show error if mail does not get sent for whatever reason.
//  if(!$sent) {
//    drupal_set_message(t('An error occurred and admin mail was not sent. Please notify administrator.'), 'error');
//  }
//
//
//  // This email is sent if the checkbox for Third Party Rental
//  // of equipment is checked.
//  if($form_state['values']['third_party_rental'] == 1) {
//    $to = $party_mail;
//    $from = $user->mail;
//    //$header_bcc = 'bcc_address@example.com';
//
//    $sent = drupal_mail('transfer_request', 'third_party', $to, language_default(), $params, $from, TRUE);
//    // Show error if mail does not get sent for whatever reason.
//    if(!$sent) {
//      drupal_set_message(t('An error occurred and admin mail was not sent. Please notify administrator.'), 'error');
//    }
//  }

  drupal_set_message(t('Thank You! Your Approval has been entered.'));
//  $form_state['redirect'] = 'transfer_request/transfer_confirmation';

}
