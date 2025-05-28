unit UnitPrincipal;

interface

  uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls;

type
  TForm1 = class(TForm)
  Label1 : TLabel;
  Label2: TLabel;
  TitulodoLivro: TEdit;
  Label3: TLabel;
  Edit2: TEdit;
  Label4: TLabel;
  ComboBox1: TComboBox;
  Label5: TLabel;
  RadioButton1: TRadioButton;
  RadioButton2: TRadioButton;
  RadioButton3: TRadioButton;
  CheckBox1: TCheckBox;
  Memo1: TMemo;
  Label6: TLabel;
  Label7: TLabel;
  CheckBox2: TCheckBox;
  CheckBox3: TCheckBox;
  CheckBox4: TCheckBox;
  Button1: TButton;
  Button2: TButton;
  Button3: TButton;
  procedure Button1Click(Sender: TObject);
  private
  { Private declarations }
  public
{ Public declarations }
end;

var
Form1 : TForm1;

implementation

{ $R * .dfm }

procedure TForm1.Button1Click(Sender: TObject);
begin
  if (TitulodoLivro.Text = 'A Lenda') then
  begin
    ShowMessage('livro Cadastrado');
  end
  else
  begin 
    if (TitulodoLivro.Text = 'Bobo') then
    begin
      ShowMessage('livro Cadastrado');
    end
    else ShowMessage('este livro nao esta no estoque');
  end
end;

end.
