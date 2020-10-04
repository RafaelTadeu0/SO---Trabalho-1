#  Declarando compilador
CC = gcc

# Flags do compilador:
#  -Wall Ativa todos os warnings
CFLAGS  = -Wall

# Declarando o target:
TARGET = pshell

README = "\n\nTrabalho da disciplina - Sistemas Operacionais\nAluno: Patrick Franco braz\nUniversidade Federal do Rio de Janeiro\nData: 16/11/2020\n\n"\
"Funcionamento do pshell:\n\n* Apos a compilacao, o executavel deve ser chamado utilizando \"./pshell\".\n* O comando que sera dado como parametro"\
"deve ser do tipo string sem nenhum caracter especial.\n* O numero de argumentos maximo esta definido como padrao"\
" em 10. Basta modificar o define para permitir aceitar mais argumentos.\n\n"

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c
	@echo $(README)

clean:
	$(RM) $(TARGET)