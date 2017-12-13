<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta content="width=device-width, initial-scale=1" name="viewport">
  <title>Log search</title>
</head>
<body>
<h1>Log search</h1>
<?php
  if(isset($_POST["query"])) { $rawquery = $_POST["query"]; }
  else { $rawquery = ""; }
  $query = preg_replace("/[^\x20-\x7e]/", "", $rawquery);
  $query = preg_replace("/(\"|\\\|\/)/", "\\\\\\1", $query);
  if(empty($query)) { $query = "*"; }

  $params = <<<EOT
{
  "from": 0,
  "size": 30,
  "query": {
    "bool": {
      "must": {
        "query_string": {
          "default_field": "request",
          "query": "{$query}"
        }
      }
    }
  },
  "sort": [
    {
      "@timestamp": {
        "order": "desc"
      }
    }
  ]
}
EOT;

  $url = "http://localhost:9200/accesslog/_search";
  $httpheader = array(
    "HTTP/1.1", "Host: localhost", "Content-Type: application/json"
  );

  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $httpheader);
  curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "GET");
  curl_setopt($ch, CURLOPT_POSTFIELDS, $params);
  $json = curl_exec($ch);
  curl_close($ch);

  $arr = json_decode($json, true);
?>
<h2>Search</h2>
<form method="POST">
<table><tr>
  <td><input type="text" name="query" value="<?php echo(htmlspecialchars($rawquery)); ?>" placeholder="Request.Path"></td>
  <td><input type="submit"></td>
</tr></table>
</form>
<h2>Result</h2>
<?php if(!array_key_exists("error", $arr)): ?>
<table border="1">
<tr>
  <th>timestamp</th>
  <th>verb</th>
  <th>request</th>
  <th>response</th>
</tr>
<?php foreach($arr["hits"]["hits"] as $val): ?>
<tr>
  <td><?php echo $val["_source"]["timestamp"]; ?></td>
  <td><?php echo $val["_source"]["verb"]; ?></td>
  <td><?php echo $val["_source"]["request"]; ?></td>
  <td><?php echo $val["_source"]["response"]; ?></td>
</tr>
<?php endforeach; ?>
</table>
<?php else: ?>
<p>Something wrong... X(</p>
<?php endif; ?>
<?php if(false): ?>
<pre>
<?php print_r($arr); ?>
</pre>
<?php endif; ?>
</body>
</html>
