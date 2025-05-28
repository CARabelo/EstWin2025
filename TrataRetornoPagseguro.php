[php]
<?php

//Verifica se foi enviado um método post
if($_SERVER[‘REQUEST_METHOD’] == ‘POST’){

//Recebe o post como o Tipo de Notificação
$tipoNotificacao = $_POST[‘notificationType’];

//Recebe o código da Notificação
$codigoNotificacao = $_POST[‘notificationCode’];

//Verificamos se tipo da notificação é transaction
if($tipoNotificacao == ‘transaction’){

//Chama a classe para utilizar a biblioteca do pagseguro
require_once "PagSeguroLibrary/PagSeguroLibrary.php";

//Informa as credenciais : Email, e TOKEN
$credencial = new PagSeguroAccountCredentials('carlosrabelo@boolesoftware.com.br', ‘bef6c570-6d8f-46c2-a37c-b8c80f85a72c2b27469740c28fbcc02edac65d99100a3996-315d-48c2-98c6-94d8445c3e76’);
 
//Verifica as informações da transação, e retorna
//o objeto Transaction com todas as informações
$transacao = PagSeguroNotificationService::checkTransaction($credencial, $codigoNotificacao);

//Retorna o objeto TransactionStatus, que vamos resgatar o valor do status
$status = $transacao->getStatus();

/**
* Verificamos se realizado o pagamento para mudar no banco de dados
* O valor 3 se referente o tipo de status, no caso informando
* que cliente realizou o pagamento.
* https://pagseguro.uol.com.br/v2/guia-de-integracao/documentacao-da-biblioteca-pagseguro-em-php.html#TransactionStatus
*/
if($status->getValue() == 3){
/**
* Pegamos o código que passamos por referência para o pagseguro
* Que no nosso exemplo é id da tabela pedido
*/
$idPedido = $transacao->getReference();

//Inclue o arquivo da conexão do banco
require_once "config/conexao.php";
/**
* Irei colocar o SQL diretamente apenas para fins didático
* Esse SQL será para mudança de status do pedido ( 2 => liberado pra envio ), e informar a hora
* da última modificação do pedido
*/
/*
$sql = "UPDATE pedidos SET status = ‘2’, modificado = NOW() WHERE id = ‘$idPedido’";
//Executa o SQL
mysql_query($sql);
*/


 
/**
* você pode depois realizar a mudança de status disparar um e-mail
* tanto para o cliente, quanto para você informando que
* já pode enviar o produto
*
*/

}
}

}
[/php]