<?php
  $galaxId = "5473";
  $galaxHash = "83Mw5u8988Qj6fZqS4Z8K7LzOo1j28S706R0BeFe";
          
 // $galaxId = "43381";  
 // $galaxHash = "Z40h2zYwEx5jDxHwOgNsC00n1gEuQpCw01IhBdRx";
  
  $url = 'https://api.sandbox.cloud.galaxpay.com.br/v2/token';
 // $url = 'https://api.galaxpay.com.br/v2/token';
  
  $header = array('Authorization: Basic ' . base64_encode($galaxId . ':' . $galaxHash)); 
  $body = '{
            "grant_type":"authorization_code",
            "scope":"customers.read customers.write plans.read plans.write transactions.read transactions.write webhooks.write balance.read balance.write cards.read cards.write card-brands.read subscriptions.read subscriptions.write boletos.read"
           }';
 
  $curl = curl_init();
  
  curl_setopt_array($curl,[
                     CURLOPT_URL => $url,
                     CURLOPT_POSTFIELDS => $body,
                     CURLOPT_RETURNTRANSFER => true,
                     CURLOPT_HTTPHEADER => $header
                           ]);
   
   $resposta = curl_exec($curl);
   
   curl_close($curl);
  
   echo '111111';
  
  
  echo "<pre>"; print_r($resposta); echo "</pre>"; 
  
  
   echo '222222';
  
  $url2 = "https://api.sandbox.cloud.galaxpay.com.br/v2/customers?startAt=0&limit=100";
  
  echo '333333';
  
  $chaves = json_decode($resposta,true);
  
  echo "<pre>"; print_r($chaves); echo "</pre>"; 
    
  $header2= "authorization:".$chaves['token_type']." ".$chaves['access_token'];
  
  echo "<pre>"; print_r($header2); echo "</pre>"; 
  
  $curl2 = curl_init();
  
  echo "444444";
  
 
  curl_setopt_array($curl2,[
                    CURLOPT_URL => $url2,
                    CURLOPT_RETURNTRANSFER => true,
                    CURLOPT_CUSTOMREQUEST => 'GET',
                    CURLOPT_HTTPHEADER => $header2
                           ]);   
                           
  echo '555555';
                        
  $resposta = curl_exec($curl2);
  
  curl_close($curl2);
  
  echo "<pre>"; print_r($resposta); echo "</pre>"; 
   
  
?>

  