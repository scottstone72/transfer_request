<?php

drupal_add_css(drupal_get_path('module', 'transfer_request') . "/dist/css/min/transfer_request_print.min.css");
drupal_add_js(drupal_get_path('module', 'transfer_request') . "/dist/js/transfer_request_form.min.js");

/**
 * Confirmation page callback function to display user submitted data and
 * printer friendly version.
 */
function transfer_confirmation_page() {
  // Check user permission
  global $user;
  if (!user_access('view transfer request form')) {
    return MENU_ACCESS_DENIED;
  }

  global $output;

  $transfer = $_SESSION['form_data']['transfer_num'];
  $date = $_SESSION['form_data']['date_submitted'];
  $trans_company = $_SESSION['form_data']['transport_company'];
  $driver = $_SESSION['form_data']['driver_name'];
  $receiver = $_SESSION['form_data']['receiver'];
  $phone = $_SESSION['form_data']['phone_num'];
  $email = $_SESSION['form_data']['email'];
  $ship_dir = $_SESSION['form_data']['shipping_directions'];
  $ship_notes = $_SESSION['form_data']['shipping_notes'];
  $to_rig = $_SESSION['form_data']['to_rig'];
  $operator = $_SESSION['form_data']['operator'];
  $to_loc = $_SESSION['form_data']['to_location'];
  $submitted_by = $_SESSION['form_data']['submitted_by'];
  $third_party = $_SESSION['form_data']['third_party_rental'];
  $transfer_items = $_SESSION['form_data']['transfer_items'];


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
      '#markup' => t('<p id="ship-directions"><strong>Shipping Directions:<br/></strong>&nbsp&nbsp @ship_dir</p>',
        array('@ship_dir' => $ship_dir)),
    ),
    'ship_notes' => array(
      '#type' => 'markup',
      '#markup' => t('<p id="ship-notes"><strong>Shipping Notes:<br/></strong>&nbsp&nbsp @date</p>',
        array('@date' => $ship_notes)),
    ),
  );

  // Horizontal rule for easier data seperation
  $output[] = array(
    '#type' => 'markup',
    '#markup' => '<div class="hr-contain"><hr class="horizontal-rule"/></div>',
  );

  // Cycle through array and add Rig, Location and Unit Code fields
  foreach ($transfer_items as $key => $value) {

    $unit = $value['unit_id'];
    $from_rig = $value['from_field'];
    $from_loc = $value['from_lsd'];
    // Set rows so we can css the every other div.
//    $key = $key + 1;
    if ($key % 2) {
      $output[] = array('#markup' => '<div class="even-row">');
    }
    else {
      $output[] = array('#markup' => '<div class="odd-row">');
    }
    $output[] = array(
      "unit_code" => array(
        '#type' => 'markup',
        '#markup' => t('<p class="unit-id"><strong>@number. Unit Code:</strong>&nbsp&nbsp @unit</p>',
          array('@unit' => $unit, '@number' => $key + 1)),
      ),
    );

    $output[] = array(
      "from_loc" => array(
        '#type' => 'markup',
        '#markup' => t('<p class="from-location"><strong><em>From Location:</em></strong>&nbsp&nbsp @from_loc</p>',
          array('@from_loc' => $from_loc)),
      ),
    );
    $output[] = array(
      "from_rig" => array(
        '#type' => 'markup',
        '#markup' => t('<p class="from-rig"><strong><em>From Rig #:</em></strong>&nbsp&nbsp @from_rig</p>',
          array('@from_rig' => $from_rig)),
      ),
    );
    $output[] = array('#markup' => '</div>');
  }

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
    '#markup' => '<div id="link-container"><ul class="link-list"><li>' . l(t(' <- back to Transfer Request Form'), 'transfer_request',
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

  // Clear the $session to keep the bugs out
//    unset($_SESSION['form_data']);
  return $output;

}

