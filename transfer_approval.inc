<?php
require_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'transfer_request') . '/includes/class.transfer_request.inc.php');
drupal_add_js(drupal_get_path('module', 'transfer_request') . "/dist/js/transfer_request_form.min.js");
drupal_add_css(drupal_get_path('module', 'transfer_request') . "/dist/css/min/transfer_request_approval.min.css");

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
    '#markup' => "<details><summary>View Instructions</summary>
      <div class='repair-message'>
      <ul>
          <li>Click <span class='color-word'>Show Details</span> to view Transfer Request.</li>
          <li>Edit form and submit to approve Transfer.</li>
      </ul>
      </div></details>",
  );


  if(!empty($trans_data)) {

    // Call our forms
    foreach ($trans_data as $key => $val) {
      $build[] = drupal_get_form('transfer_request_approval_form_' . $val['transfer_num'], $val);
    }

  } else {
    $build['no_val'] = array(
      '#type' => 'markup',
      '#markup' => '<div>- There are no Transfer Request Approvals at this time.</div>',
    );
  }

  return $build;
}

/**
 * Transfer_request_approval_form
 */
function transfer_request_approval_form($form, &$form_state, $args) {

  // Check for user permission
  if (!user_access('view transfer approvals and submissions')) {
    return MENU_ACCESS_DENIED;
  }
  global $user;
  // Grab passed in arguments
  $transfer_data = $args;

  $transfer_num = $transfer_data['transfer_num'];
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
  $third_party = $transfer_data['third_party_rental'];

  $transfer_items = unserialize($transfer_data['transfer_items']);

  // Create header array for table
  $header = array(
    'transfer_num' => t('Transfer#'),
    'more_details' => t('Details'),
    'date_submitted' => t('Date'),
    'receiver' => t('Receiver'),
    'third_party' => t('Third Party Rental'),
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
    'third_party' => !empty($third_party) ? 'Yes' : 'No',
    'submitted_by' => $submitted_by,
  );

  // Construct the table
  $form['approval_table_' . $transfer_num] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#attributes' => array('class' => array('approval-table')),
    '#js_select' => FALSE,
    '#empty' => t('No users found'),
    '#multiple' => TRUE,
  );

  $form['#tree'] = TRUE;

  $form['transfer_order_' . $transfer_num] = array(
    '#type' => 'fieldset',
    '#id' => 'transfer-order-' . $transfer_num,
    '#attributes' => array('class' => array('approval-fieldset')),
  );

  $form['transfer_order_' . $transfer_num]['transfer_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Transfer #'),
    '#id' => 'transfer-num',
    '#value' => $transfer_num,
    '#size' => 11,
    '#disabled' => TRUE,
    '#prefix' => '<div class="transfer-num">',
    '#suffix' => '</div>',
  );

  $form['transfer_order_' . $transfer_num]['date'] = array(
    '#type' => 'textfield',
    '#title' => t('Date'),
    '#value' => $date_submitted,
    '#size' => 30,
    '#disabled' => TRUE,
    '#prefix' => '<div class="date-submitted">',
    '#suffix' => '</div>',
  );

  $form['transfer_order_' . $transfer_num]['submitted_by'] = array(
    '#type' => 'textfield',
    '#title' => t('Submitted by'),
    '#value' => $submitted_by,
    '#disabled' => TRUE,
    '#prefix' => '<div class="submitted-by">',
    '#suffix' => '</div>',
  );
  $form['transfer_order_' . $transfer_num]['contact_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Contact Info'),
    '#id' => 'contact-info',
  );
  $form['transfer_order_' . $transfer_num]['contact_info']['receiver'] = array(
    '#type' => 'textfield',
    '#title' => t('Receiver'),
    '#default_value' => $receiver,
    '#required' => TRUE,
    '#prefix' => '<div class="receiver">',
    '#suffix' => '</div>',
  );
  $form['transfer_order_' . $transfer_num]['contact_info']['phone_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone Number'),
    '#default_value' => $phone_num,
    '#prefix' => '<div class="phone-num">',
    '#suffix' => '</div>',
  );
  $form['transfer_order_' . $transfer_num]['contact_info']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#default_value' => $email,
    '#prefix' => '<div class="email">',
    '#suffix' => '</div>',
  );
  $form['transfer_order_' . $transfer_num]['contact_info']['trans_company'] = array(
    '#type' => 'textfield',
    '#title' => t('Transport Company'),
    '#default_value' => $transport_company,
    '#prefix' => '<div class="trans-company">',
    '#suffix' => '</div>',
  );
  $form['transfer_order_' . $transfer_num]['contact_info']['drivers_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Drivers Name'),
    '#default_value' => $driver_name,
    '#prefix' => '<div class="driver-name">',
    '#suffix' => '</div>',
  );


  $form['transfer_order_' . $transfer_num]["transfer_items"] = array(
    '#type' => 'fieldset',
    '#title' => t('Transfer Items'),
  );


  // Add our Transfer Items to the form array
  foreach ($transfer_items as $key => $value) {

    $form['transfer_order_' . $transfer_num]["transfer_items"][$key]["item_num"] = array(
      '#type' => 'textfield',
      '#title' => 'Item #',
      '#disabled' => TRUE,
      '#default_value' => $key + 1,
      '#id' => 'item-' . $key,
      '#prefix' => '<div class="transfer-items-num">',
      '#suffix' => '</div>',
    );

    $form['transfer_order_' . $transfer_num]["transfer_items"][$key]['unit_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Unit ID'),
      '#id' => 'unit-id-' . $key,
      '#disabled' => TRUE,
      '#default_value' => $value['unit_id'],
      '#prefix' => '<div class="unit-id">',
      '#suffix' => '</div>',
    );

    $form['transfer_order_' . $transfer_num]["transfer_items"][$key]['from_field'] = array(
      '#type' => 'textfield',
      '#title' => t('From'),
      '#description' => t('Rig #'),
      '#default_value' => $value['from_field'],
      '#disabled' => TRUE,
      '#prefix' => '<div class="from-rig">',
      '#suffix' => '</div>',
    );
    $form['transfer_order_' . $transfer_num]["transfer_items"][$key]['from_lsd'] = array(
      '#type' => 'textfield',
      '#title' => t('Current Location'),
      '#description' => t('L.S.D.'),
      '#default_value' => $value['from_lsd'],
      '#disabled' => TRUE,
      '#prefix' => '<div class="from-lsd">',
      '#suffix' => '</div>',
    );


    $trans_data[$key] = array(
      'nid' => $value['nid'],
      'unit_id' => $value['unit_id'],
      'from_field' => $value['from_field'],
      'from_lsd' => $value['from_lsd'],
    );

    $form['transfer_data'] = array(
      '#type' => 'value',
      '#value' => !empty($trans_data) ? $trans_data : array(),
    );
  }

  $form['transfer_order_' . $transfer_num]['destination'] = array(
    '#type' => 'fieldset',
    '#title' => t('Destination'),
  );

  // To field
  $form['transfer_order_' . $transfer_num]['destination']['to_field'] = array(
    '#type' => 'textfield',
    '#title' => t('To Rig'),
    '#description' => t('Rig #'),
    '#default_value' => $to_rig,
    '#required' => TRUE,
    '#prefix' => '<div class="to-rig">',
    '#suffix' => '</div>',
  );

  // To LSD
  $form['transfer_order_' . $transfer_num]['destination']['to_lsd'] = array(
    '#type' => 'textfield',
    '#title' => t('To Location'),
    '#description' => t('L.S.D.'),
    '#default_value' => $to_location,
    '#required' => TRUE,
    '#prefix' => '<div class="to-lsd">',
    '#suffix' => '</div>',
  );

  // Operator
  $form['transfer_order_' . $transfer_num]['destination']['operator'] = array(
    '#type' => 'textfield',
    '#title' => t('Operator'),
    '#default_value' => $operator,
    '#required' => TRUE,
    '#prefix' => '<div class="operator">',
    '#suffix' => '</div>',
  );

  $form['transfer_order_' . $transfer_num]['directions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Directions and Notes'),
    '#id' => 'directions',
  );
  $form['transfer_order_' . $transfer_num]['directions']['ship_directions'] = array(
    '#type' => 'textarea',
    '#title' => t('Shipping Directions'),
    '#default_value' => $shipping_directions,
    '#prefix' => '<div class="ship-directions">',
    '#suffix' => '</div>',
  );
  $form['transfer_order_' . $transfer_num]['directions']['shipping_notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#default_value' => $shipping_notes,
    '#prefix' => '<div class="ship-notes">',
    '#suffix' => '</div>',
  );

  // Build drop down fieldset to hold input elements
  $form['transfer_order_' . $transfer_num]['admin_section'] = array(
    '#type' => 'fieldset',
    '#title' => t('Admin Section'),
    '#id' => 'admin-section',
  );
  $form['transfer_order_' . $transfer_num]['admin_section']['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes:'),
    '#prefix' => '<div class="admin-notes">',
    '#suffix' => '</div>',
  );
  $form['transfer_order_' . $transfer_num]['admin_section']['admin_approver'] = array(
    '#type' => 'textfield',
    '#title' => t('Administrator name:'),
    '#disabled' => TRUE,
    '#value' => $user->name,
    '#prefix' => '<div class="admin-approver">',
    '#suffix' => '</div>',
  );

  $form['transfer_order_' . $transfer_num]['third_party_rental'] = array(
    '#type' => 'checkbox',
    '#title' => t('Applicable Third Party Charges'),
    '#id' => 'third-party-checkbox',
    '#default_value' => $third_party,
    '#prefix' => '<div id="third-party" class="checkbox">',
    '#suffix' => '</div>',
  );

  $form['transfer_order_' . $transfer_num]['container'] = array(
    '#type' => 'container',
  );

  $form['transfer_order_' . $transfer_num]['container']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Approve Transfer'),
    '#id' => 'submit-button-' . $transfer_num,
    '#attributes' => array('class' => array('btn btn-primary')),
    '#prefix' => '<div class="submit-button">',
    '#suffix' => '</div>',
  );

  $form['transfer_order_' . $transfer_num]['container']['cancel'] = array(
    '#markup' => '<div><span value="' . $transfer_num . '"  class="cancel-btn-val">Hide Form</span></div>',
    '#prefix' => '<div class="cancel-button">',
    '#suffix' => '</div>',
  );

  return $form;
}

/**
 * Validate the form data
 */
function transfer_request_approval_form_validate($form, &$form_state) {
  // Serialize our transfer data for submission
  $form_state['values']['transfer_data'] = serialize($form_state['values']['transfer_data']);
}

/**
 * Submit the data to the database.
 */
function transfer_request_approval_form_submit($form, &$form_state) {

  $transfer_num = array();
  // Get each triggering element number off end of triggering elements #id
  preg_match('/(\d+)$/', $form_state['triggering_element']['#id'], $transfer_num);
  // grab first string value
  $transfer_num = $transfer_num[0];


  $fields = array(
    'transfer_num' => $form_state['values']['transfer_order_' . $transfer_num]['transfer_num'],
    'date_submitted' => $form_state['values']['transfer_order_' . $transfer_num]['date'],
    'submitted_by' => $form_state['values']['transfer_order_' . $transfer_num]['submitted_by'],
    'receiver' => $form_state['values']['transfer_order_' . $transfer_num]['contact_info']['receiver'],
    'phone_num' => $form_state['values']['transfer_order_' . $transfer_num]['contact_info']['phone_num'],
    'email' => $form_state['values']['transfer_order_' . $transfer_num]['contact_info']['email'],
    'transport_company' => $form_state['values']['transfer_order_' . $transfer_num]['contact_info']['trans_company'],
    'driver_name' => $form_state['values']['transfer_order_' . $transfer_num]['contact_info']['drivers_name'],
    'shipping_directions' => $form_state['values']['transfer_order_' . $transfer_num]['directions']['ship_directions'],
    'shipping_notes' => $form_state['values']['transfer_order_' . $transfer_num]['directions']['shipping_notes'],
    'transfer_items' => $form_state['values']['transfer_data'],
    'to_rig' => $form_state['values']['transfer_order_' . $transfer_num]['destination']['to_field'],
    'operator' => $form_state['values']['transfer_order_' . $transfer_num]['destination']['operator'],
    'to_location' => $form_state['values']['transfer_order_' . $transfer_num]['destination']['to_lsd'],
    'admin_notes' => $form_state['values']['transfer_order_' . $transfer_num]['admin_section']['notes'],
    'administrator' => $form_state['values']['transfer_order_' . $transfer_num]['admin_section']['admin_approver'],
    'third_party_rental' => $form_state['values']['transfer_order_' . $transfer_num]['third_party_rental'],
    'approved' => 'approved',
  );

  db_update('transfer_request_updated')
    ->fields($fields)
    ->condition('transfer_num', $transfer_num)
    ->execute();

  // Start a SESSION to store the user input for the confirmation print page to use.
  $_SESSION['approval_data'] = $fields;

  // Need to update update_transfer_node_fields() function
  update_transfer_node_fields($fields);

  // Clear the field_data_fields cache
  $cid = 'field:node:';
  cache_clear_all($cid, 'cache_field', TRUE);

  drupal_set_message(t('Thank You! Your Approval has been entered.'));
  $form_state['redirect'] = 'transfer_request_approval/print/' . $transfer_num;

}

// This function is for updating the field node tables
function update_transfer_node_fields($tr_data) {

  // Unserialize transfer items.
  $transfer_items = unserialize($tr_data['transfer_items']);
  $transfer_data = $tr_data;

  for($i = 0; $i < count($transfer_items); $i++) {
    unset($transfer_items[$i]['from_field']);
    unset($transfer_items[$i]['from_lsd']);
    unset($transfer_items[$i]['unit_id']);
  }

  // Get rid of what we don't need for submitting to the node tables
  unset($transfer_data['transfer_items']);
  unset($transfer_data['transfer_num']);
  unset($transfer_data['submitted_by']);
  unset($transfer_data['driver_name']);
  unset($transfer_data['shipping_directions']);
  unset($transfer_data['shipping_notes']);
  unset($transfer_data['admin_notes']);
  unset($transfer_data['administrator']);
  unset($transfer_data['third_party_rental']);
  unset($transfer_data['approved']);


  $trans_items_arr = array();
  // Add $transfer_data to each transfer item for submitting and updating to node tables below.
  foreach ($transfer_items as $key => $item) {
    $trans_items_arr[$key] = $item + $transfer_data;
  }

  // Start submitting to node fields
  foreach ($trans_items_arr as $key => $value) {

    // Update the field_data_field_date table with new field_date_value
    db_merge('field_data_field_date_out')
      ->key(array(
        'entity_type' => 'node',
        'bundle' => 'rental_product',
        'entity_id' => $value['nid'],
      ))
      ->fields(array(
        'field_date_out_value' => $value['date_submitted'],
        'deleted' => 0,
        'revision_id' => 0,
        'language' => 'und',
        'delta' => 0,
        'field_date_out_format' => NULL,
      ))
      ->execute();

    // Update the field_data_field_rig table with new field_rig_value
    db_merge('field_data_field_rig')
      ->key(array(
        'entity_type' => 'node',
        'bundle' => 'rental_product',
        'entity_id' => $value['nid'],
      ))
      ->fields(array(
        'field_rig_value' => $value['to_rig'],
        'deleted' => 0,
        'revision_id' => 0,
        'language' => 'und',
        'delta' => 0,
        'field_rig_format' => NULL,
      ))
      ->execute();

    // Update the field_data_field_location table with new field_location_value
    db_merge('field_data_field_location')
      ->key(array(
        'entity_type' => 'node',
        'bundle' => 'rental_product',
        'entity_id' => $value['nid'],
      ))
      ->fields(array(
        'field_location_value' => $value['to_location'],
        'deleted' => 0,
        'revision_id' => 0,
        'language' => 'und',
        'delta' => 0,
        'field_location_format' => NULL,
      ))
      ->execute();

    // Update the field_data_field_contact table with new field_contact_value
    db_merge('field_data_field_contact')
      ->key(array(
        'entity_type' => 'node',
        'bundle' => 'rental_product',
        'entity_id' => $value['nid'],
      ))
      ->fields(array(
        'field_contact_value' => $value['receiver'],
        'deleted' => 0,
        'revision_id' => 0,
        'language' => 'und',
        'delta' => 0,
        'field_contact_format' => NULL,
      ))
      ->execute();

    // Update the field_data_field_operator table with new field_operator_value
    db_merge('field_data_field_operator')
      ->key(array(
        'entity_type' => 'node',
        'bundle' => 'rental_product',
        'entity_id' => $value['nid'],
      ))
      ->fields(array(
        'field_operator_value' => $value['operator'],
        'deleted' => 0,
        'revision_id' => 0,
        'language' => 'und',
        'delta' => 0,
        'field_operator_format' => NULL,
      ))
      ->execute();

    // Update the field_data_field_contact_phone_number table with new field_contact_phone_number_value
    db_merge('field_data_field_contact_phone_number')
      ->key(array(
        'entity_type' => 'node',
        'bundle' => 'rental_product',
        'entity_id' => $value['nid'],
      ))
      ->fields(array(
        'field_contact_phone_number_value' => $value['phone_num'],
        'deleted' => 0,
        'revision_id' => 0,
        'language' => 'und',
        'delta' => 0,
        'field_contact_phone_number_format' => NULL,
      ))
      ->execute();

    // Update the field_data_field_contact_email_address table with new field_contact_email_address_email
    db_merge('field_data_field_contact_email_address')
      ->key(array(
        'entity_type' => 'node',
        'bundle' => 'rental_product',
        'entity_id' => $value['nid'],
      ))
      ->fields(array(
        'field_contact_email_address_email' => $value['email'],
        'deleted' => 0,
        'revision_id' => 0,
        'language' => 'und',
        'delta' => 0,
      ))
      ->execute();

    // Update the field_data_field_contact_phone_number table with new field_contact_phone_number_value
    db_merge('field_data_field_transport_company')
      ->key(array(
        'entity_type' => 'node',
        'bundle' => 'rental_product',
        'entity_id' => $value['nid'],
      ))
      ->fields(array(
        'field_transport_company_value' => $value['transport_company'],
        'deleted' => 0,
        'revision_id' => 0,
        'language' => 'und',
        'delta' => 0,
        'field_transport_company_format' => NULL,
      ))
      ->execute();
  }


}