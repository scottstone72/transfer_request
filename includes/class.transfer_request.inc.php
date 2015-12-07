<?php

class TransferRequest {

  /**
   * Get serial number to fill form field
   * @return array|mixed
   */
  public function transfer_value() {

    $query = db_select('transfer_request_updated', 'tr');
    $query->addExpression('MAX(transfer_num)');
    $max= $query->execute()->fetchField();

    if(empty($max)) {
      $form_state['values']['contact_info']['transfer_number'] = 1;
      return $form_state['values']['contact_info']['transfer_number'];
    }
    else {
      //$serial = end($serial);
      $form_state['values']['contact_info']['transfer_number'] = $max;
      return $form_state['values']['contact_info']['transfer_number'] + 1;
    }
  }

  /**
   * Get all Unit_ID values for select field
   * @return mixed
   */
  public function unit_id_value() {

    $unit_id_select_options = array();

    $query = db_select('node', 'n');
    $query->join('field_data_field_unit_type_', 'f', 'n.nid = f.entity_id');
    $query->groupBy('f.entity_id');
    $query->join('taxonomy_term_data', 't', 'f.field_unit_type__tid = t.tid');
    $query->groupBy('t.tid');
    $query->fields('t', array('name'));
    $query->fields('n', array('title','type', 'nid'));
    $query->fields('f', array('field_unit_type__tid'));
    $results = $query->execute();


    foreach($results as $val) {
      $unit_id_select_options[$val->name][$val->nid] = $val->title;
    }

    return $unit_id_select_options;
  }


  /**
   * Populate Rig and Location fields when user selects Unit ID
   */
  public function get_rig_location_values($nid, $item_num = 1) {

    $query = db_select('node', 'n');
    $query->join('field_data_field_rig',
      'r', 'r.entity_id = n.nid'
    );
    $query->join('field_data_field_location ',
      'l',
      'n.nid = l.entity_id AND
     r.entity_id = n.nid'
    );

    $results = $query
      ->fields('n', array('nid', 'title'))
      ->fields('r', array('field_rig_value', 'entity_id'))
      ->fields('l', array('field_location_value', 'entity_id'))
      ->condition('nid', $nid)
      ->execute();

    $rig_local = array();
    // Use item number to help us organize
    // the object sent for each item number.
    foreach($results as $value) {
      $rig_local[] = $value;
    }

    return $rig_local;
  }

  /**
   * Get Transfer Request Approval data
   */
  public function get_transfer_approval_data() {

    // Query the database.
    $query = db_select('transfer_request_updated', 'tr')
      ->fields('tr')
      ->condition('approved', 'FALSE')
      ->orderBy('transfer_num', 'DESC')
      ->execute();

    $order_arr = array();

    while($order = $query->fetchAssoc()) {
      $order_arr[] = $order;
    }

    return $order_arr;
  }

  /**
   * Get Transfer Request Approval data
   */
  public function get_transfer_archive_data() {

    // Query the database.
    $query = db_select('transfer_request_updated', 'tr')
      ->fields('tr')
      ->condition('approved', 'approved')
      ->orderBy('transfer_num', 'DESC')
      ->execute();

    $order_arr = array();

    while($order = $query->fetchAssoc()) {
      $order_arr[] = $order;
    }

    return $order_arr;
  }

  /**
   * Select a single repair item from database.
   */
  public function get_transfer_request_item($transfer_num = NULL) {
    $transfer_item = db_select('transfer_request_updated', 'tr')
      ->fields('tr')
      ->condition('transfer_num', $transfer_num)
      ->execute()
      ->fetchAssoc();
    return $transfer_item;
  }
}

