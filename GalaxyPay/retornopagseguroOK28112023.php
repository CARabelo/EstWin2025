<!DOCTYPE html>
<html lang="pt-br">
<head>
</head>

<body style="background: ;">

<?php
// Certifique-se de que este script aceita requisições com Content-Type: application/json
header('Content-Type: application/json');

// Lê o corpo da requisição e decodifica o JSON
$inputJSON = file_get_contents('php://input');
$input = json_decode($inputJSON, TRUE); // converte para array associativo

//--- Converte o status para os status do webservice da Boole
//--- Não tenho todos os status ainda... verificar

$statustemp =  $input['status'] ?? null;

$transstatus = "";

  switch ($transstatus) {
    case "completed":
         $transstatus = "3";
    break;
         
    case "cancelled" : 
        $transstatus = "2";
    break;
          
    default:
        $transaction_status = "0";
    break;
  }

$urlSite = 'https://boolesoftware.com.br/';
  
if($transstatus == "7") //--- Desistiu ou erro, não completou o pagamento
{
  header("Location: erro-pagamento");
  die();
}
  
    /*   === STATUS ANTIGOS DO PAGSEGURO ===

     1	Aguardando pagamento: o comprador iniciou a transação, mas até o momento o PagSeguro não recebeu nenhuma informação sobre o pagamento.
     2	Em análise: o comprador optou por pagar com um cartão de crédito e o PagSeguro está analisando o risco da transação.
     3	Paga: a transação foi paga pelo comprador e o PagSeguro já recebeu uma confirmação da instituição financeira responsável pelo processamento.
     4	Disponível: a transação foi paga e chegou ao final de seu prazo de liberação sem ter sido retornada e sem que haja nenhuma disputa aberta.
     5	Em disputa: o comprador, dentro do prazo de liberação da transação, abriu uma disputa.
     6	Devolvida: o valor da transação foi devolvido para o comprador.
     7	Cancelada: a transação foi cancelada sem ter sido finalizada. */
     
  //--- 3 (e 4) são os únicos em que o pagamento foi realmente efetuado (CC)
  //--- Manda um get para o webservice cadastrar o cliente e mandar-lhe um email 
  //--- com o link para download e com o número de serie.
  
  if($transstatus == "1" || $transstatus == "2" || $transstatus == "3" || $transstatus == "4")   
  {
    if (!is_null($input)) 
    {
      $dataWS['VendedorEmail'] = "Site BooleSoftware LTDA";
      $dataWS['TransacaoID'] =   $input['order_key'] ?? null;
      $dataWS['Referencia'] =  $input['parent_id'] ?? null;
      $dataWS['Extras'] =  $input['total_tax'] ?? null;
      $dataWS['TipoFrete'] =  $input['method_title'] ?? null;
      $dataWS['ValorFrete'] =   $input['shipping_total'] ?? null;
      $dataWS['Anotacao'] =  $input['pament_method'] ?? null;  
      $dataWS['DataTransacao'] =  $input['date_paid'] ?? null; 
      $dataWS['TipoPagamento'] = $input['payment_method_title'] ?? null;
      $dataWS['StatusTransacao'] = $input['status'] ?? null;   
      $dataWS['CliNome'] = $input['billing']['first_name'] + $input['billing']['last_name'] ?? null;  
      $dataWS['CliEmail'] =  $input['billing']['email'] ?? null;
      $dataWS['CliEndereco'] = $input['billing']['address_1'] ?? null;
      $dataWS['CliNumero'] =  $input['billing']['address_2'] ?? null;
      $dataWS['Clicidade'] = $input['billing']['city'] ?? null;   
      $dataWS['CliEstado'] = $input['billing']['state'] ?? null; 
      $dataWS['CliCEP'] = $input['billing']['postcode'] ?? null;
      $dataWS['cliTelefone'] = $input['billing']['phone'] ?? null;
      $dataWS['ProdID_x'] = $input['meta_data']['line_items']['productid'] ?? null;  
      $dataWS['ProdDescricao_x'] = $input['meta_data']['line_items']['name'] ?? null; 
      $dataWS['ProdQuantidade_x'] = $input['meta_data']['line_items']['quantity'] ?? null;
      $dataWS['ProdValor_x'] = $input['total'] ?? null;
      $dataWS['ProdFrete_x'] =  $input['total_tax'] ?? null;
      $dataWS['NumItens'] =$input['quantity'] ?? null;
      $dataWS['valorTotal'] = $input['total'] ?? null;
      $dataWS['taxaPagSeguro'] = '0.00';
      $dataWS['dataultimoevento'] = $input['date_modified'] ?? null;

      // Salva os dados em um arquivo de log (opcional)

      $arquivo = "loggalaxypay.txt";
      $fp = fopen($arquivo, "a+");
      fwrite($fp,'=======================================================================\n');
      fwrite($fp, json_encode($dataWS));
      fclose($fp);

      //--- Manda os dados para o WebService da Boole Software
    
      $urlWS = 'http://148.72.64.68/webservice-viasurbanas-estradas/webservice/notificacoes'.$transaction_id.'?'.$dataWS;
 
      $curlWS = curl_init();

      curl_setopt($curlWS,CURLOPT_RETURNTRANSFER,true);
      curl_setopt($curlWS,CURLOPT_URL,$urlWS);
 
      $xmlWS = curl_exec($curlWS);

      curl_close($curlWS);
    }
  }
  
  //--- Direciona o cliente para a respectiva página da boole

  $cliente = $dataWS['CliNome'];
  $email = $dataWS['CliEmail'];
  $chave =  $dataWS['TransacaoID'];
       
  if($transstatus == "1" || $transstatus == "2" || $transstatus == "3" || $transstatus == "4")   //--- Compra confirmada ou aguardando pagamento
  {  
    $parameters = '?cliente='.$cliente.'&email='.$email;
    
    if($transstatus == "1" || $transstatus == "2")
    {
      header("Location: pedido-recebido".$parameters);   //---Página aguardando o pagamento
      die();
    }
    
    if($transstatus == "3" || $transstatus == "4")    //--- Compra confirmada
    {
      $parameters = '?cliente='.$cliente.'&email='.$email.'&chave='.$chave.'&num_cliente='.$num_cliente;
      header("Location: compra-confirmada".$parameters);    //--- A compra foi confirmada
      die();
    }
  }
  
  //--- Desistiu de pagar
 
  $parameters = '?cliente='.$cliente.'&email='.$email;
  header("Location: pacotes-vias-urbanas-estradas".$parameters);       //--- Não pagou, vai pra pag VU&E
  die();
?>
</body>
</html>

//-----------------------
$galaxId = "43381";
$galaxHash = "Z40h2zYwEx5jDxHwOgNsC00n1gEuQpCw01IhBdRx";
curl_setopt($ch, CURLOPT_HTTPHEADER, array('Authorization: Basic ' . base64_encode( $galaxId . ':' . $galaxHash) ) );

$jsonRet = json_decode(curl_exec($ch));