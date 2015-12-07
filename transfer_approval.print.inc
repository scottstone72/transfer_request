<?php

drupal_add_css(drupal_get_path('module', 'transfer_request') . "/dist/css/transfer_request_print.css");
drupal_add_js(drupal_get_path('module', 'transfer_request') . "/dist/js/transfer_request_form.min.js");

/**
 * Confirmation page callback function to display user submitted data and
 * printer friendly version.
 */
function transfer_request_approval_print() {
  // Check user permission
  global $user;
  if (!user_access('view transfer request form')) {
    return MENU_ACCESS_DENIED;
  }

  global $output;

  $transfer = $_SESSION['approval_data']['transfer_num'];
  $date = $_SESSION['approval_data']['date_submitted'];
  $trans_company = $_SESSION['approval_data']['transport_company'];
  $driver = $_SESSION['approval_data']['driver_name'];
  $receiver = $_SESSION['approval_data']['receiver'];
  $phone = $_SESSION['approval_data']['phone_num'];
  $email = $_SESSION['approval_data']['email'];
  $ship_dir = $_SESSION['approval_data']['shipping_directions'];
  $ship_notes = $_SESSION['approval_data']['shipping_notes'];
  $admin_notes = $_SESSION['approval_data']['admin_notes'];
  $admin_approver = $_SESSION['approval_data']['administrator'];
  $to_rig = $_SESSION['approval_data']['to_rig'];
  $operator = $_SESSION['approval_data']['operator'];
  $to_loc = $_SESSION['approval_data']['to_location'];
  $submitted_by = $_SESSION['approval_data']['submitted_by'];
  $third_party = $_SESSION['approval_data']['third_party_rental'];
  $transfer_items = $_SESSION['approval_data']['transfer_items'];


  $transfer_items = unserialize($transfer_items);

  // Send image through the theme_image function.
  $image_path = drupal_get_path('theme', 'ponderosa_bootstrap') . '/images/ponderosa_logo.png';
  $image = array(
    'path' => $image_path,
    'alt' => 'Company Logo',
    'title' => 'Company Logo',
    'width' => '150px',
    'attributes' => array('class' => 'company-image', 'id' => 'company-image'),
  );

  $img = theme('image', $image);

  $output[] = array(
    'company_logo' => array(
      '#markup' => '<div>' . $img . '</div>',
    ),
  );

    $output[] = array('#markup' => '<div id="print-content">');

  $output[] = array(
    'heading' => array(
      '#type' => 'markup',
      '#markup' => t('<h1 id="title-top"><strong>Transfer Request</strong></h1>'),
    ),
    'transfer_num' => array(
      '#type' => 'markup',
      '#markup' => t('<h2 id="transfer-num"><strong>Transfer Order #</strong>@transfer</h2>',
        array('@transfer' => $transfer)),
    ),
    'date' => array(
      '#type' => 'markup',
      '#markup' => t('<p id="date"><strong>Date:</strong>&nbsp&nbsp @date</p>',
        array('@date' => $date)),
    ),
    'receiver' => array(
      '#type' => 'markup',
      '#markup' => t('<p id="receiver"><strong>Receiver:</strong>&nbsp&nbsp @receiver</p>',
        array('@receiver' => $receiver)),
    ),
    'phone' => array(
      '#type' => 'markup',
      '#markup' => t('<p id="phone"><strong>Phone:</strong>&nbsp&nbsp @phone</p>',
        array('@phone' => $phone)),
    ),
    'email' => array(
      '#type' => 'markup',
      '#markup' => t('<p id="email"><strong>Email:</strong>&nbsp&nbsp !email</p>',
        array('!email' => $email)),
    ),
    'trans_company' => array(
      '#type' => 'markup',
      '#markup' => t('<p id="trans-company"><strong>Transport Company:</strong>&nbsp&nbsp @trans_company</p>',
        array('@trans_company' => $trans_company)),
    ),
    'driver' => array(
      '#type' => 'markup',
      '#markup' => t('<p id="driver"><strong>Driver:</strong>&nbsp&nbsp @driver</p>',
        array('@driver' => $driver)),
    ),
    'ship_dir' => array(
      '#type' => 'markup',
      '#markup' => t('<p id="ship-directions"><strong>Shipping Directions:<br/></strong>@ship_dir</p>',
        array('@ship_dir' => $ship_dir)),
    ),
    'ship_notes' => array(
      '#type' => 'markup',
      '#markup' => t('<p id="ship-notes"><strong>Shipping Notes:<br/></strong>@ship_notes</p>',
        array('@ship_notes' => $ship_notes)),
    ),
    'admin_approver' => array(
      '#type' => 'markup',
      '#markup' => t('<p id="admin-approver"><strong>Approved by: </strong>@admin_approver</p>',
        array('@admin_approver' => $admin_approver)),
    ),
    'admin_notes' => array(
      '#type' => 'markup',
      '#markup' => t('<p id="admin-notes"><strong>Admin Notes:<br/></strong>@admin_notes</p>',
        array('@admin_notes' => $admin_notes)),
    ),
  );

  // Horizontal rule for easier data seperation
  $output[] = array(
    '#type' => 'markup',
    '#markup' => '<div class="hr-contain"><hr class="horizontal-rule"/></div>',
  );

    $output[] = array('#markup' => '<table id="print-table">');
    $output[] = array('#markup' => '<thead><tr><th>Unit Code</th><th>From Rig</th><th>From Location</th></tr></thead><tbody>');

  // Cycle through array and add Rig, Location and Unit Code fields
  foreach ($transfer_items as $key => $value) {

    $unit = $value['unit_id'];
    $from_rig = $value['from_field'];
    $from_loc = $value['from_lsd'];
    // Set rows so we can css the every other div.
//    $key = $key + 1;
    if ($key % 2) {
      $output[] = array('#markup' => '<tr class="even-row">');
    }
    else {
      $output[] = array('#markup' => '<tr class="odd-row">');
    }
    $output[] = array(
      "unit_code" => array(
        '#type' => 'markup',
        '#markup' => t('<td class="unit-id"><strong>@number. @unit</td>',
          array('@unit' => $unit, '@number' => $key + 1)),
      ),
    );
    $output[] = array(
      "from_rig" => array(
        '#type' => 'markup',
        '#markup' => t('<td class="from-rig"> @from_rig</td>',
          array('@from_rig' => $from_rig)),
      ),
    );
    $output[] = array(
      "from_loc" => array(
        '#type' => 'markup',
        '#markup' => t('<td class="from-location"> @from_loc</td>',
          array('@from_loc' => $from_loc)),
      ),
    );
    $output[] = array('#markup' => '</tr>');
  }
    // Close the table
    $output[] = array('#markup' => '</tbody></table>');

  // Horizontal rule for easier data seperation
  $output[] = array(
    '#type' => 'markup',
    '#markup' => '<div class="hr-contain"><hr class="horizontal-rule"/></div>',
  );

  $output[] = array(
    'to_rig' => array(
      '#type' => 'markup',
      '#markup' => t('<p id="dest-rig"><strong>To Rig #:</strong>&nbsp&nbsp @to_rig</p>',
        array('@to_rig' => $to_rig)),
    ),
    'to_loc' => array(
      '#type' => 'markup',
      '#markup' => t('<p id="dest-location"><strong>To Location:</strong>&nbsp&nbsp @to_loc</p>',
        array('@to_loc' => $to_loc)),
    ),
    'operator' => array(
      '#type' => 'markup',
      '#markup' => t('<p id="operator"><strong>Operator:</strong>&nbsp&nbsp @to_operator</p>',
        array('@to_operator' => $operator)),
    ),
    'submitted_by' => array(
      '#type' => 'markup',
      '#markup' => t('<p id="submitted-by"><strong>Submitted By:</strong>&nbsp&nbsp @submitted_by</p>',
        array('@submitted_by' => $submitted_by)),
    ),
  );

  if ($third_party == 1) {
    $output[] = array(
      '#type' => 'markup',
      '#markup' => t('<p id="third-party"><strong>Applicable third party charges required.</strong></p><br>'),
    );
  }


  $output[] = array(
    '#type' => 'markup',
    '#markup' => '<div id="link-container"><ul class="link-list"><li>' . l(t(' <- back to Transfer Request Approvals'), 'transfer_request_approval',
        array('attributes' => array('class' => array('transfer-form-link')))) . "</li>",
  );

  $output[] = array(
    '#type' => 'markup',
    '#markup' => '<li>' . l(t('Home'), '', array('attributes' => array('class' => array('home-link')))) . "</li>",
  );

  $output[] = array(
    '#type' => 'markup',
    '#markup' => '<li id="print-link"><div id="print-this-page">Print Page</div></li></ul></div>',
  );

    $output[] = array('#markup' => '</div>');// End print-content

  // Clear the $session to keep the bugs out
//    unset($_SESSION['form_data']);
  return $output;

}

