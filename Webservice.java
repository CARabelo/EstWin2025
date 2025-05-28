package com.boolesoftware.webservice;

import org.jboss.resteasy.annotations.jaxrs.PathParam;
import org.jboss.resteasy.annotations.jaxrs.FormParam;

import com.boolesoftware.DTO.ChaveDto;
import com.boolesoftware.DTO.UsuarioPagSeguroDto;

import io.quarkus.mailer.Mail;
import io.quarkus.mailer.Mailer;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Application;

import org.eclipse.microprofile.openapi.annotations.OpenAPIDefinition;
import org.eclipse.microprofile.openapi.annotations.info.Info;

import java.text.ParseException;
import java.text.SimpleDateFormat;

import javax.inject.Inject;
import javax.transaction.Transactional;

import io.quarkus.mailer.Mailer;
import io.quarkus.mailer.Mail;

@Path("")
@OpenAPIDefinition(
    info = @Info(
        description = "Endpoint para verificação e instalação de licenças",
        title="API de licenças",
        version="0.0.1"
    )
)
public class Webservice extends Application{

    @Inject
    WebServiceResource webResource;
    
    @Inject
    Mailer mailer;

    @GET
    @Path("/webservice/{acao}/{codigo_cliente}/{chave}")
    @Produces(value=MediaType.APPLICATION_JSON)
    public ChaveDto processarMensagem(@PathParam int acao, @PathParam int codigo_cliente, @PathParam String chave){
        
    	ChaveDto chaveDto = new ChaveDto();
        chaveDto.setAcao(acao);
        chaveDto.setCodigo_cliente(codigo_cliente);
        chaveDto.setChave(chave);
        
        if((chaveDto.getChave() == null) || (chaveDto.getChave().isEmpty())){
            chaveDto.setCodigo_resposta(3);
            chaveDto.setMensagem("Chave invalida");
            return chaveDto;
        }

        if(chaveDto.getCodigo_cliente() == 0){
            chaveDto.setCodigo_resposta(6);
            chaveDto.setMensagem("Codigo do cliente invalido");
            return chaveDto;
        }

        switch (chaveDto.getAcao()) {
            case 1:
                chaveDto.setStatus(webResource.verificarChave(chaveDto.getChave()));

                if(chaveDto.getStatus() == -1){
                    chaveDto.setCodigo_resposta(5);
                    chaveDto.setMensagem("Chave não encontrada");
                } else if(!webResource.verificarCliente(chaveDto.getChave(), chaveDto.getCodigo_cliente())){
                    chaveDto.setCodigo_resposta(6);
                    chaveDto.setMensagem("Esta chave nao pertence a este cliente");
                } else if(chaveDto.getStatus() == 0){
                    chaveDto.setCodigo_resposta(0);
                    chaveDto.setMensagem("Chave criada");
                } else if(chaveDto.getStatus() == 1){
                    chaveDto.setCodigo_resposta(4);
                    chaveDto.setMensagem("Chave instalada");
                } else if(chaveDto.getStatus() == 2){
                    chaveDto.setCodigo_resposta(6);
                    chaveDto.setMensagem("Chave desistalada");
                } else if(chaveDto.getStatus() == 3){
                    chaveDto.setCodigo_resposta(2);
                    chaveDto.setMensagem("Chave cancelada");
                }

                break;
            case 2:
                chaveDto.setStatus(webResource.verificarChave(chaveDto.getChave()));

                if(chaveDto.getStatus() == -1){
                    chaveDto.setCodigo_resposta(5);
                    chaveDto.setMensagem("Chave não encontrada");
                } else if(chaveDto.getStatus() == 1){
                    chaveDto.setCodigo_resposta(6);
                    chaveDto.setMensagem("Esta chave ja foi instalada");
                } else if(!webResource.verificarCliente(chaveDto.getChave(), chaveDto.getCodigo_cliente())){
                    chaveDto.setCodigo_resposta(6);
                    chaveDto.setMensagem("Esta chave nao pertence a este cliente");
                } else if(webResource.atualizarStatus(chaveDto.getChave(), 1)){
                    chaveDto.setCodigo_resposta(4);
                    chaveDto.setMensagem("Chave instalada com sucesso");
                } else {
                    chaveDto.setCodigo_resposta(6);
                    chaveDto.setMensagem("Erro ao instalada chave. Entre em contato com o suporte tecnico");
                }
                break;
            default:
                chaveDto.setCodigo_resposta(6);
                chaveDto.setMensagem("Acao invalida");
                break;
        }

        return chaveDto;
    }

    @GET
    @Path("/webservice/{acao}/{codigo_cliente}/{chave}/{novo_status}")
    @Produces(value=MediaType.APPLICATION_JSON)
    public ChaveDto processarMensagem(@PathParam int acao, @PathParam int codigo_cliente, @PathParam String chave, @PathParam int novo_status){
        
    	ChaveDto chaveDto = new ChaveDto();
        chaveDto.setAcao(acao);
        chaveDto.setCodigo_cliente(codigo_cliente);
        chaveDto.setChave(chave);
        
        if((chaveDto.getChave() == null) || (chaveDto.getChave().isEmpty())){
            chaveDto.setCodigo_resposta(3);
            chaveDto.setMensagem("Chave invalida");
            return chaveDto;
        }

        if(chaveDto.getCodigo_cliente() == 0){
            chaveDto.setCodigo_resposta(6);
            chaveDto.setMensagem("Codigo do cliente invalido");
            return chaveDto;
        }

        switch (chaveDto.getAcao()) {

            case 3:
                chaveDto.setStatus(webResource.verificarChave(chaveDto.getChave()));

                if(chaveDto.getStatus() == -1){
                    chaveDto.setCodigo_resposta(5);
                    chaveDto.setMensagem("Chave não encontrada");
                } else if(!webResource.verificarCliente(chaveDto.getChave(), chaveDto.getCodigo_cliente())){
                    chaveDto.setCodigo_resposta(6);
                    chaveDto.setMensagem("Esta chave nao pertence a este cliente");
                } else if(webResource.atualizarStatus(chaveDto.getChave(), novo_status)){
                    chaveDto.setCodigo_resposta(4);
                    chaveDto.setMensagem("Chave atualizada com sucesso");
                    chaveDto.setStatus(novo_status);
                } else {
                    chaveDto.setCodigo_resposta(6);
                    chaveDto.setMensagem("Erro ao atualizada chave. Entre em contato com o suporte tecnico");
                }
                
                break;
            default:
                chaveDto.setCodigo_resposta(6);
                chaveDto.setMensagem("Acao invalida");
                break;
        }

        return chaveDto;
    }
    
    @POST
    @Path("/notificacoes")
    @Consumes(MediaType.APPLICATION_FORM_URLENCODED)
    @Produces(value=MediaType.TEXT_PLAIN)
    @Transactional
    public Response notificacoes(
    		@FormParam("VendedorEmail") String VendedorEmail,
    		@FormParam("TransacaoID") String TransacaoID,
    		@FormParam("Referencia") String Referencia,
    		@FormParam("Extras") Double Extras,
    		@FormParam("TipoFrete") String TipoFrete,
    		@FormParam("ValorFrete") Double ValorFrete,
    		@FormParam("Anotacao") String Anotacao,
    		@FormParam("DataTransacao") String DataTransacao,
    		@FormParam("TipoPagamento") String TipoPagamento,
    		@FormParam("StatusTransacao") String StatusTransacao,
    		@FormParam("CliNome") String CliNome,
    		@FormParam("CliEmail") String CliEmail,
    		@FormParam("CliEndereco") String CliEndereco,
    		@FormParam("CliNumero") String CliNumero,
    		@FormParam("CliComplemento") String CliComplemento,
    		@FormParam("CliBairro") String CliBairro,
    		@FormParam("CliCidade") String CliCidade,
    		@FormParam("CliEstado") String CliEstado,
    		@FormParam("CliCEP") String CliCEP,
    		@FormParam("CliTelefone") String CliTelefone,
    		@FormParam("ProdID_x") String ProdID_x,
    		@FormParam("ProdDescricao_x") String ProdDescricao_x,
    		@FormParam("ProdValor_x") Double ProdValor_x,
    		@FormParam("ProdQuantidade_x") int ProdQuantidade_x,
    		@FormParam("ProdFrete_x") Double ProdFrete_x,
    		@FormParam("NumItens") int NumItens) {
    	
    	SimpleDateFormat sdf = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss");
    	
    	UsuarioPagSeguroDto usuario = new UsuarioPagSeguroDto();
    	usuario.setVendedorEmail(VendedorEmail);
    	usuario.setTransacaoID(TransacaoID);
    	usuario.setReferencia(Referencia);
    	usuario.setExtras(Extras);
    	usuario.setTipoFrete(TipoFrete);
    	usuario.setValorFrete(ValorFrete);
    	usuario.setAnotacao(Anotacao);
    	usuario.setTipoPagamento(TipoPagamento);
    	usuario.setStatusTransacao(StatusTransacao);
    	usuario.setCliNome(CliNome);
    	usuario.setCliEmail(CliEmail);
    	usuario.setCliEndereco(CliEndereco);
    	usuario.setCliNumero(CliNumero);
    	usuario.setCliComplemento(CliComplemento);
    	usuario.setCliBairro(CliBairro);
    	usuario.setCliCidade(CliCidade);
    	usuario.setCliEstado(CliEstado);
    	usuario.setCliCEP(CliCEP);
    	usuario.setCliTelefone(CliTelefone);
    	usuario.setProdID_x(ProdID_x);
    	usuario.setProdDescricao_x(ProdDescricao_x);
    	usuario.setProdQuantidade_x(ProdQuantidade_x);
    	usuario.setProdFrete_x(ProdFrete_x);
    	usuario.setNumItens(NumItens);
    	usuario.setProdValor_x(ProdValor_x);
    	
    	try {
			usuario.setDataTransacao(new java.sql.Date(sdf.parse(DataTransacao).getTime()));
		} catch (ParseException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	
    	webResource.inserirUsuarioPagSeguro(usuario);
    	int idCliente = webResource.inserirCliente(CliNome, CliEndereco, CliTelefone, CliNome, 0, CliEmail);
    	String chaveVirgem = webResource.getChaveVirgem();
    	if(chaveVirgem != null) {
    		webResource.inserirChave(chaveVirgem, 0, idCliente, 1, "");
    		body = body.replaceAll("##chave##", chaveVirgem).replaceAll("##idcliente##", String.valueOf(idCliente));
    		mailer.send(Mail.withText(CliEmail,"Chave do produto", body));
    	}
    	
    	return Response.accepted().build();
    }
    
    private String body = "Prezado cliente,\n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			"Seguem abaixo instruções para instalação do programa Vias Urbanas & Estradas:\n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			"Link para baixar o programa de instalação:\n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			"www.boolesoftware.com.br/download/link_a_definir\n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			"Número de série (Necessário para a instalação do programa):\n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			"##chave##  \n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			"1 – Clique no link acima para baixar o programa de instalação;\n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			"2 – Assim que o arquivo estiver baixado, dê um click duplo no arquivo para iniciar a instalação.\n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			"3 – Siga as instruções do programa de instalação.\n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			"4 – ao terminar a instalação o programa estará pronto para uso.\n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			"Seu número de usuário é: ##idcliente##\n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			"Atenção:\n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			"Posteriormente você pode precisar dos dados contidos neste email  portanto guarde-o  com muito cuidado.\n" + 
			"\n" + 
			"Os dados contidos aqui caracterizam o seu direito ao uso da cópia do programa Vias Urbanas & estradas,\n" + 
			"\n" + 
			"que lhe está sendo enviada.\n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			"[blocked::http://www.boolesoftware.com.br/]\n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			"     Madalena T. R. Silva\n" + 
			"\n" + 
			"             Diretora comercial​\n" + 
			"\n" + 
			"                      (11) 2598-7013\n" + 
			"\n" + 
			"        www.boolesoftware.com.br\n" + 
			"\n" + 
			"                         Rua Paraná 190 \n" + 
			"\n" + 
			"                            CEP 09669-010\n" + 
			"\n" + 
			" São Bernardo do Campo - SP\n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			"Não responda este e-mail, os e-mail enviados a este endereço serão perdidos e não serão respondidos.\n" + 
			"\n" + 
			"Se você recebeu este mensagem por enganado, favor eliminá-la imediatamente e por gentileza envie um e-mail à comercial@boolesoftware.com.br comunicando este fato. Agradecemos.\n" + 
			"\n" + 
			"\"Esta mensagem é reservada e sua divulgação, distribuição, reprodução ou qualquer forma de uso é proibida e depende de prévia autorização da Boole Software. \n" + 
			"\n" + 
			"O remetente utiliza o correio eletrônico da Boole Software no exercício do seu trabalho ou em razão dele. Caso este email contenha qualquer tipo de abuso ou ofensa favor comunicar este \n" + 
			"\n" + 
			"fato imediatamente à Boole Software LTDA através do email  relacionamento@boolesoftware.com.br.\n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			"Todos os direitos autorais 2010-2020 estão reservados à Boole  Software Ltda. – Brasil\n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			" \n" + 
			"\n" + 
			" ";
}