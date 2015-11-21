<?php
require_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'transfer_request') . '/includes/class.transfer_request.inc.php');
drupal_add_js(drupal_get_path('module', 'transfer_request') . "/dist/js/transfer_request_form.min.js");
drupal_add_css(drupal_get_path('module', 'transfer_request') . "/dist/css/min/transfer_request_approval.min.css");

/**
 * Show form once callback has fired.
 *
 * @return mixed
 */
function transfer_request_archive_page() {
//  $build = array();
  $rows = array();

  // Get our db object
  $transfer_obj = new TransferRequest();
  $trans_data = $transfer_obj->get_transfer_archive_data();

  $build = "<details><summary>View Instructions</summary>
      <div class='repair-message'>
      <ul>
          <li>Click <span class='color-word'>Show Details</span> to view Archived Transfer Request.</li>
          <li>Click <span class='color-word'>Print Preview</span> to show Transfer Request Order and then click <span class='color-word'>Print Page</span> for a printed copy.</li>
      </ul>
      </div></details>";

  if(!empty($trans_data)) {
    // Call our forms
    foreach ($trans_data as $key => $val) {

      $forms = drupal_get_form('transfer_request_archive_form_' . $val['transfer_num'], $val);

      // Build the rows and add data
      $rows[] = array(drupal_render($forms));
    }

  } else {
    $build['no_val'] = array(
      '#type' => 'markup',
      '#markup' => '<div>- There are no Archived Transfer Requests at this time.</div>',
    );
  }



  $per_page = 5;
  // Initialize the pager
  $current_page = pager_default_initialize(count($rows), $per_page);
  // Split your list into page sized chunks
  $chunks = array_chunk($rows, $per_page, TRUE);

  $build .= theme_table(
      array(
        "header" => array(),
        "rows" => $chunks[$current_page],
        "attributes" => array('class' => array('funkytable')),
        "sticky" => false, // Table header will be sticky
        "caption" => "",
        "colgroups" => array(),
        "empty" => t("Table has no row!") // The message to be displayed if table is empty
      )
    ).theme("pager", array('quantity',count($rows)));

  return $build;
}

/**
 * Transfer_request_approval_form
 */
function transfer_request_archive_form($form, &$form_state, $args) {

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
  $admin_approver = $transfer_data['administrator'];
  $admin_notes = $transfer_data['admin_notes'];

  $transfer_items = unserialize($transfer_data['transfer_items']);

  // Create header array for table
  $header = array(
    'transfer_num' => t('Transfer#'),
    'more_details' => t('Details'),
    'date_submitted' => t('Date'),
    'receiver' => t('Receiver'),
    'submitted_by' => t('Submitted by'),
    'print' => t('Print a Copy'),
  );

  // Link for modal forms
  $links['pop_form'] = '<span value="' . $transfer_num . '" class="show-details">Show Details</span>';
  $links['print'] = '<a href="/transfer_request_archive/print/' . $transfer_num . '">Print Preview</a>';

  $rows = array();
  // Build the rows and add data
  $rows[$transfer_num] = array(
    'transfer_num' => $transfer_num,
    'more_details' => $links['pop_form'],
    'date_submitted' => $date_submitted,
    'receiver' => $receiver,
    'submitted_by' => $submitted_by,
    'print' => $links['print'],
  );

  // Construct the table
  $form['approval_table_' . $transfer_num] = array(
    '#type' => 'tableselect',
//    '#theme' => 'pager',
    '#header' => $header,
    '#options' => $rows,
    '#attributes' => array('class' => array('archived-table')),
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
    '#disabled' => TRUE,
    '#prefix' => '<div class="receiver">',
    '#suffix' => '</div>',
  );
  $form['transfer_order_' . $transfer_num]['contact_info']['phone_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone Number'),
    '#default_value' => $phone_num,
    '#disabled' => TRUE,
    '#prefix' => '<div class="phone-num">',
    '#suffix' => '</div>',
  );
  $form['transfer_order_' . $transfer_num]['contact_info']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#default_value' => $email,
    '#disabled' => TRUE,
    '#prefix' => '<div class="email">',
    '#suffix' => '</div>',
  );
  $form['transfer_order_' . $transfer_num]['contact_info']['trans_company'] = array(
    '#type' => 'textfield',
    '#title' => t('Transport Company'),
    '#default_value' => $transport_company,
    '#disabled' => TRUE,
    '#prefix' => '<div class="trans-company">',
    '#suffix' => '</div>',
  );
  $form['transfer_order_' . $transfer_num]['contact_info']['drivers_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Drivers Name'),
    '#default_value' => $driver_name,
    '#disabled' => TRUE,
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
      //      '#description' => t('Rig #'),
      '#default_value' => $value['from_field'],
      '#disabled' => TRUE,
      '#prefix' => '<div class="from-rig">',
      '#suffix' => '</div>',
    );
    $form['transfer_order_' . $transfer_num]["transfer_items"][$key]['from_lsd'] = array(
      '#type' => 'textfield',
      '#title' => t('Current Location'),
      //      '#description' => t('L.S.D.'),
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
    //    '#description' => t('Rig #'),
    '#default_value' => $to_rig,
    '#disabled' => TRUE,
    '#prefix' => '<div class="to-rig">',
    '#suffix' => '</div>',
  );

  // To LSD
  $form['transfer_order_' . $transfer_num]['destination']['to_lsd'] = array(
    '#type' => 'textfield',
    '#title' => t('To Location'),
    //    '#description' => t('L.S.D.'),
    '#default_value' => $to_location,
    '#disabled' => TRUE,
    '#prefix' => '<div class="to-lsd">',
    '#suffix' => '</div>',
  );

  // Operator
  $form['transfer_order_' . $transfer_num]['destination']['operator'] = array(
    '#type' => 'textfield',
    '#title' => t('Operator'),
    '#default_value' => $operator,
    '#disabled' => TRUE,
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
    '#disabled' => TRUE,
    '#prefix' => '<div class="ship-directions">',
    '#suffix' => '</div>',
  );
  $form['transfer_order_' . $transfer_num]['directions']['shipping_notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#default_value' => $shipping_notes,
    '#disabled' => TRUE,
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
    '#value' => $admin_notes,
    '#disabled' => TRUE,
    '#prefix' => '<div class="admin-notes">',
    '#suffix' => '</div>',
  );
  $form['transfer_order_' . $transfer_num]['admin_section']['admin_approver'] = array(
    '#type' => 'textfield',
    '#title' => t('Administrator name:'),
    '#value' => $admin_approver,
    '#disabled' => TRUE,
    '#prefix' => '<div class="admin-approver">',
    '#suffix' => '</div>',
  );

  $form['transfer_order_' . $transfer_num]['third_party_rental'] = array(
    '#type' => 'checkbox',
    '#title' => t('Applicable Third Party Charges'),
    '#id' => 'third-party-checkbox',
    '#default_value' => $third_party,
    '#disabled' => TRUE,
    '#prefix' => '<div id="third-party" class="checkbox">',
    '#suffix' => '</div>',
  );

  $form['transfer_order_' . $transfer_num]['container'] = array(
    '#type' => 'container',
  );

  $form['transfer_order_' . $transfer_num]['container']['cancel'] = array(
    '#markup' => '<div><span value="' . $transfer_num . '"  class="cancel-btn-val">Hide Form</span></div>',
    '#prefix' => '<div class="cancel-button">',
    '#suffix' => '</div>',
  );

  return $form;
}
