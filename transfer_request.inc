<?php
require_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'transfer_request') . '/includes/class.transfer_request.inc.php');
drupal_add_css(drupal_get_path('module', 'transfer_request') . '/dist/css/min/transfer_request_form.min.css');

/**
 * Show form once callback has fired.
 * @return mixed
 */
function transfer_request_form_page() {
  global $user;

  if(!user_access('view transfer request form')) {
    return MENU_ACCESS_DENIED;
  }
  return drupal_get_form('transfer_request_form');
}

/**
 * Form render array for transfer_request_form_page callback
 * @param $form
 * @param $form_state
 * @return array
 */
function transfer_request_form($form, &$form_state) {

  global $user;
  $_user = $user->name;
  $match = array();
  $is_unit_trigger = array();

  $transfer_obj = new TransferRequest();
  $transfer_num = $transfer_obj->transfer_value();
  $unit_id_select = $transfer_obj->unit_id_value();

  $form = array();
  $form['#tree'] = TRUE;

  $form['field_instructions'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="required-container"><strong>Fields</strong> marked with a
      <strong><span class="required">red</span> asterick = " <span class="required">*</span> "</strong> are required.</div>',
  );

  # Contact information field.
  $form['contact_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Contact Information'),
  );
  # Serial number
  $form['contact_info']['transfer_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Transfer #'),
    '#value' => $transfer_num,
    '#size' => 11,
    '#disabled' => TRUE,
    '#prefix' => '<div id="transfer-num">',
    '#suffix' => '</div>',
  );
  # Date field
  $form['contact_info']['date'] = array(
    '#type' => 'textfield',
    '#title' => t('Date'),
    '#value' => date('M d, Y'),
    '#size' => 30,
    '#disabled' => TRUE,
    '#prefix' => '<div id="date-submitted">',
    '#suffix' => '</div>',
  );
  # Contact Info
  $form['contact_info']['submitted_by'] = array(
    '#type' => 'textfield',
    '#title' => t('Submitted by'),
    '#value' => $_user,
    '#disabled' => TRUE,
    '#prefix' => '<div id="submitted-by">',
    '#suffix' => '</div>',
  );


  # Unit ID selector, Rig and From LSD fields
  $form["location"] = array(
    '#type' => 'fieldset',
    '#title' => t("Originating Location"),
    '#id' => 'ajax-transfer-items-container',
//    '#attributes' => array('class' => array('row')),
  );



  // $form_state['location']['items'] to determine the number of textfields to build.
  if (empty($form_state['location']['items'])) {
    $form_state['location']['items'] = 1;
  }

  for ($i = 0; $i < $form_state['location']['items']; $i++) {

    // Grab the word unit form triggering element to use to filter which button was selected
    $triggering_ele = isset($form_state['triggering_element']['#id']) ? $form_state['triggering_element']['#id'] : '';
    preg_match('/(\w)*/', $triggering_ele, $is_unit_trigger); // Get the first word of unit #id

    // Get each triggering element number off end of triggering elements #id
    $trigger = isset($form_state['triggering_element']['#id']) ? $form_state['triggering_element']['#id'] : '';
    preg_match('/(\d+)$/', $trigger, $match);
    $match = array_shift($match);

    if(is_numeric($match) && $is_unit_trigger[0] == 'unit') {

      $nid = isset($form_state['values']["location"]['items'][$i]["unit_id_select"])
        ? $form_state['values']["location"]['items'][$i]["unit_id_select"] : '';

      // Grab the data, send nid and item number as params
      $rig_location = $transfer_obj->get_rig_location_values($nid, $match);

      // Save nid and any other data for validate and submit function
      $form_state['transfer_data'][$i]['unit_id'][] = $rig_location[$i]->title;

      // Assign data to $form_state
      $form_state['values']['location']['items'][$i]['data']['from_field'][] =
        $rig_location[$i]->field_rig_value;

      $form_state['values']['location']['items'][$i]['data']['from_lsd'][] =
        $rig_location[$i]->field_location_value;
  }

    $form_state['transfer_values'] = array(
      '#type' => 'value',
      '#value' => '',
    );

    $form["location"]['items'][$i]["item_num"] = array(
      '#type' => 'textfield',
      '#title' => 'Item #',
      '#default_value' => $i + 1,
      '#id' => 'item-' . $i,
      '#prefix' => '<div class="transfer-items-num">',
      '#suffix' => '</div>',
    );

    $form["location"]['items'][$i]["unit_id_select"] = array(
      '#type' => 'select',
      '#title' => t('Unit ID'),
      '#empty_option' => t('- Select Unit ID -'),
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

    $form["location"]['items'][$i]['data'] = array(
      '#type' => 'container',
      '#id' => 'rig-location-data-' . $i,
    );


    $form["location"]['items'][$i]['data']["from_field"] = array(
      '#type' => 'textfield',
      '#title' => t('Rig #'),
      '#default_value' => isset($form_state['values']['location']['items'][$i]['data']['from_field'])
        ? $form_state['values']['location']['items'][$i]['data']['from_field'] : NULL,
      '#id' => "from-field-" . $i,
      '#disabled' => TRUE,
      //      '#attributes' => array('class' => array('transfer-items')),
      '#prefix' => '<div class="from-field">',
      '#suffix' => '</div>',
    );
    $form["location"]['items'][$i]['data']["from_lsd"] = array(
      '#type' => 'textfield',
      '#title' => t('Current Location'),
      '#default_value' => isset($form_state['values']['location']['items'][$i]['data']['from_lsd'])
        ? $form_state['values']['location']['items'][$i]['data']['from_lsd'] : NULL,
      '#description' => t('L.S.D.'),
      '#id' => "from-lsd-" . $i,
      '#disabled' => TRUE,
      //      '#attributes' => array('class' => array('transfer-items')),
      '#prefix' => '<div class="from-lsd">',
      '#suffix' => '</div>',
    );
  }



  // Submit button to add more repairs text fields
  $form['add_repair'] = array(
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
  $form['remove_repair'] = array(
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
    $form['dest_location'] = array(
      '#type' => 'fieldset',
      '#title' => t('Destination Location'),
      '#prefix' => '<div id="dest-location">',
      '#suffix' => '</div>',
    );
    // To field
    $form['dest_location']['to_field'] = array(
      '#type' => 'textfield',
      '#title' => t('Rig #'),
      '#required' => TRUE,
      '#prefix' => '<div id="to-field">',
      '#suffix' => '</div>',
    );
  $form['dest_location']['operator'] = array(
    '#type' => 'textfield',
    '#title' => t('Operator'),
    '#required' => TRUE,
    '#prefix' => '<div id="operator">',
    '#suffix' => '</div>',
  );
    // To LSD
    $form['dest_location']['to_lsd'] = array(
      '#type' => 'textfield',
      '#title' => t('Location'),
      '#description' => t('L.S.D.'),
      '#required' => TRUE,
      '#prefix' => '<div id="to-lsd">',
      '#suffix' => '</div>',
    );
    // Shipping details
    $form['ship_details'] = array(
      '#type' => 'fieldset',
      '#title' => t('Shipping Details'),
    );
    $form['ship_details']['trans_company'] = array(
      '#type' => 'textfield',
      '#title' => t('Transport Company'),
      '#prefix' => '<div id="trans-company">',
      '#suffix' => '</div>',
    );
    $form['ship_details']['drivers_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Drivers Name'),
      '#prefix' => '<div id="drivers-name">',
      '#suffix' => '</div>',
    );
    $form['ship_details']['receiver'] = array(
      '#type' => 'textfield',
      '#title' => t('Receiver'),
      '#required' => TRUE,
      '#prefix' => '<div id="receiver">',
      '#suffix' => '</div>',
    );
    $form['ship_details']['phone_num'] = array(
      '#type' => 'textfield',
      '#title' => t('Phone Number'),
      '#prefix' => '<div id="phone-num">',
      '#suffix' => '</div>',
    );
    $form['ship_details']['receiver_email'] = array(
      '#type' => 'textfield',
      '#title' => t('Email'),
      '#prefix' => '<div id="receiver-email">',
      '#suffix' => '</div>',
    );
    $form['ship_details']['ship_directions'] = array(
      '#type' => 'textarea',
      '#title' => t('Shipping Directions'),
      '#prefix' => '<div id="ship-directions">',
      '#suffix' => '</div>',
    );
    $form['ship_details']['shipping_notes'] = array(
      '#type' => 'textarea',
      '#title' => t('Notes'),
      '#prefix' => '<div id="shipping-notes">',
      '#suffix' => '</div>',
    );

  $form['third_party_rental'] = array(
    '#type' => 'checkbox',
    '#title' => t('Applicable Third Party Charges'),
    '#id' => 'third-party-checkbox',
    '#prefix' => '<div id="third-party" class="checkbox">',
    '#suffix' => '</div>',
  );

    // Submit button
    $form['submit_button'] = array(
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

  return $form["location"]['items'][$item]['data'];
}


// Ajax function called and used to rebuild form
function ajax_transfer_order_callback($form, &$form_state) {

  return $form['location'];
}

/**
 * Submit handler for the "add repair" button.
 *
 * Increments the repairs_req fields and causes a rebuild.
 */
function transfer_order_add_textfield($form, &$form_state) {

  $form_state['location']['items']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove repair" button.
 *
 * Decrements the repairs_req fields and causes a form rebuild.
 */
function transfer_order_remove_textfield($form, &$form_state) {

  if ($form_state['location']['items'] > 1) {
    $form_state['location']['items']--;
  }
  $form_state['rebuild'] = TRUE;
}


/**
 * Validate the form data
 * @param $form
 * @param $form_state
 */
function transfer_request_form_validate($form, &$form_state) {

  $trans_val = array();

  // If email is supplied then validate it
  if(!empty($form_state['values']['ship_details']['receiver_email'])) {
    $mail = $form_state['values']['ship_details']['receiver_email'];
    // Pass result to $answer variable
    $answer = valid_email_address($mail);

    if($answer == FALSE) {
      form_set_error('ship_details][receiver_email',
        t('Please enter a proper email address!'));
    }
  }

  // Organize our values for the submit function
  for ($t = 0; $t < $form_state['location']['items']; $t++) {

    $nid = $form_state['values']['location']['items'][$t]['unit_id_select'];

    $trans_val[$t]['nid'] = $nid;
    $trans_val[$t] ['unit_id'] = $form_state['transfer_data'][$t]['unit_id'][0];
    $trans_val[$t]['from_field'] = $form_state['values']['location']['items'][$t]['data']['from_field'][0];
    $trans_val[$t]['from_lsd'] = $form_state['values']['location']['items'][$t]['data']['from_lsd'][0];
  }
  dsm($trans_val);
  // Serialize and store our transfer_values back to form_state
  $form_state['transfer_values'] = serialize($trans_val);

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
      'timestamp' => time(),
  );

  $query = db_insert('transfer_request_updated')
    ->fields($fields);

  // Submit to database
  $query->execute();

  // Start a SESSION to store the user input for the confirmation print page to use.
  $_SESSION['form_data'] = $fields;


  global $user;
  // Get emails that have been set in the transfer request configuration page.
  $admin_mail = variable_get('transfer_admin_email');
  $party_mail = variable_get('transfer_party_email');
  //$cc = variable_get('transfer_cc_email');

  $date = $form_state['values']['contact_info']['date'];
  // Send email to administrator notifying him of request.
  $to = $admin_mail;
  $from = $user->mail;
  //$header_cc = $cc;
  $params['transfer_num'] =  $transfer_num;
  $params['date'] = $date;
  $params['notes'] = $form_state['values']['ship_details']['shipping_notes'];

  $sent = drupal_mail('transfer_request', 'new_transfer', $to, language_default(), $params, $from, TRUE);
  // Show error if mail does not get sent for whatever reason.
  if(!$sent) {
      drupal_set_message(t('An error occurred and admin mail was not sent. Please notify administrator.'), 'error');
    }


  // This email is sent if the checkbox for Third Party Rental
  // of equipment is checked.
  if($form_state['values']['third_party_rental'] == 1) {
    $to = $party_mail;
    $from = $user->mail;
    //$header_bcc = 'bcc_address@example.com';

    $sent = drupal_mail('transfer_request', 'third_party', $to, language_default(), $params, $from, TRUE);
    // Show error if mail does not get sent for whatever reason.
    if(!$sent) {
      drupal_set_message(t('An error occurred and admin mail was not sent. Please notify administrator.'), 'error');
    }
  }

  drupal_set_message(t('Thank You! Your Approval has been entered.'));
  $form_state['redirect'] = 'transfer_request/print';

}
