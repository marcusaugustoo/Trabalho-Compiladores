# X++ Compiler – Compiladores I (UFMS)

Projeto desenvolvido para a disciplina **Compiladores I**, ministrada pela **Profa. Bianca de Almeida Dantas** na **UFMS/FACOM (2025/2)**.  
O objetivo é implementar as duas primeiras etapas do compilador da linguagem **X++**:

- **Análise Léxica**: reconhecimento de tokens e ignorância de espaços e comentários.  
- **Análise Sintática**: validação da estrutura do código conforme a gramática da linguagem.

O compilador foi implementado em **C++** e lê arquivos com a extensão `.xpp`, exibindo mensagens de erro ou informando a compilação bem-sucedida.

---

## ⚙️ Compilação

```bash
g++ *.cpp -o xpp_compiler
```

---

## ▶️ Execução

```bash
./xpp_compiler arquivo.xpp
```

Exemplo:
```bash
./xpp_compiler teste1.xpp
```

---

## 👥 Autores

- **Caio K. F. Mendes**  
- **Marcus Augusto Ferreira Madureira**
