
CREATE PROCEDURE `atualiza_mensalidades`()
BEGIN
  DECLARE hoje date default curdate();
  DECLARE nome_arq_mensalidades varchar(255) default '/home/carlos/pag_seguro/arqtranspagseguro' + hoje + '.txt';
  DECLARE mensalidades_cursor CURSOR  for SELECT * FROM mensalidades  WHERE data > HOJE;
  DECLARE data_processamento datetime;
  DECLARE nomeplano varchar(32); 
  DECLARE chave_lida varchar(36);
  DECLARE data date;
  DECLARE tracker varchar(32); 
  DECLARE status_lido varchar(32); 
  DECLARE referencia varchar(32); 
  DECLARE data_ult_evento date;
  DECLARE tipo_pagamento varchar(32); 
  DECLARE erro varchar(16);  
  
  SELECT CONCAT('/home/carlos/pag_seguro/arqtranspagseguro',hoje,'.txt') as temp;
  LOAD DATA LOCAL INFILE '/home/carlos/pag_seguro/arqtranspagsegurohoje.txt' INTO TABLE mensalidades;
    
  OPEN mensalidades_cursor;  
  FETCH NEXT FROM vend_cursor INTO data_processamento, nomeplano, chave_lida, data, tracker, status_lido, referencia, data_ult_evento, tipo_pagamento, erro;
 
  WHILE @@FETCH_STATUS = 0 DO
    IF 	tipo_pagamento = 'AUTO' THEN
	  UPDATE mensalidades
	    SET data_vencimento = prox_vencimento , data_ult_atualizacao = HOJE
	  WHERE chave = chave_lida;
    END IF;
  END WHILE;
  CLOSE mensalidades_cursor;
END  
