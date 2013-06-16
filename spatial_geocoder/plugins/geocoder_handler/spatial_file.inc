<?php
/**
 * Plugin to provide a spatial file geocoder.
 */

$plugin = array(
  'title' => t('Spatial file'),
  'description' => t('Get the geometry out of shapefiles and kml'),
  'callback' => 'spatial_geocoder_file',
  'field_types' => array('file'),
  'field_callback' => 'spatial_geocoder_field',
);

function spatial_geocoder_file($wkt_features) {
  geophp_load();
  $store_features = array();
  $wkt = array();

  foreach ($wkt_features as $wkt_feature) {
    if (!isset($wkt_feature['WKT']) || empty($wkt_feature)) {
      continue;
    }
    $store_features[] = $wkt_feature['WKT'];
  }

  if ($store_features) {
    // If there is more than one value to save, wrap it up in a GeometryCollection
    if (isset($store_features[1])) {
      $wkt = sprintf('GEOMETRYCOLLECTION(%s)', implode(',', $store_features));
    } else {
      $wkt = $store_features[0];
    }
  }

  return geoPHP::load($wkt, 'wkt');
}

function spatial_geocoder_field($field, $field_item) {
  if ($field['type'] == 'file') {
    if ($field_item['fid']) {
      $file = file_load($field_item['fid']);
      $features = spatial_file_get_wkt_features($file->uri);

      return spatial_geocoder_file($features);
    } else {
      // If there was no file we want to pass an empty value.
      return spatial_geocoder_file(array());
    }
  }
}
