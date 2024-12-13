/*
    Atividade: Projeto Autoral
    Algoritmos e Programação II - TADS II
    Aluno: Ricardo Hideki Tsugikava Junior
    Prof. Berssa

    Jogo - Um computador antigo...

    ATENÇÃO!!!!!!
    O programa utiliza alguns comandos que estão apenas disponíveis em sistemas windows.
    Tente utilizar um compilador ou um programa que leia o código no sistema operacional indicado.

    Programa feito no Visual Studio 2022.
*/

#include <iostream> 
#include <locale.h> //Localizar textos
#include <Windows.h> //Biblioteca para funcionalidades do windows
#include <sstream> //Biblioteca para formatação de strings por espaços
#include <string> //Biblioteca para formatação e utilização de strings
#include <vector> //Biblioteca de vetores
#include <memory> //Biblioteca para melhor utilização da memória
#include <fstream> //Biblioteca para manipulação de arquivos

using namespace std;

struct arquivos { //Declaração de um struct de arquivos - O struct de arquivos é uma boa escolha no lugar de diversos array por deixar o código mais limpo e vísivel.
    string arquivoNome; 
    int textoArrayTamanho;
    vector<string> texto;
};

//Declaração dos arquivos.
arquivos problemas;
arquivos enigma;
arquivos ola;

//Declaração de constantes sobre certos delays que o programa usa.
const int delaySimulation = 500, maisDelaySimulation = 1000, delayMudandoTela = 2000;

//<--------------------------- Textos gerados pelo programa --------------------------->
//Utilização de vetores por se trataram de uma forma mais dinâmica de array, não precisando ditar seu tamanho e sendo mais simples sua manipulação.
const vector<string> textosProblemas = {
    "Eu não entendo como tal coisa poderia existir. Já vi muitas coisas que os humanos do mundo achariam que se trata apenas de uma ficção mal feita, mas isto não é desse mundo.\n\n",
    "Não acredito que alguém chegará ler essas palavras póstumas, eu também espero que não.\n\n",
    "Se alguém ainda sim estiver lendo, deixe esta coisa no local onde encontrou, destruir não irá pará-lo.\n\n",
    "Espero que a Divisão saiba o que fazer após esse enorme fracasso que foi essa missão...\n\n",
    "Consigo ouvir seu coração mais alto\n\n\n",
    "Ele está aqui\n\n"
};

const vector<string> textoEnigma = {
    "Qual animal tem quatro patas pela manhã, duas pela tarde e à noite três patas?\n\n",
    "dica: a resposta é só uma palavra.\n\n"
};

const vector<string> textoEncriptado = {
    "Atrás de você\n\n",
    ":)\n\n\n"
};

//<--------------------------- Funções sobre manipulação de arquivos --------------------------->
// A utilização desses arquivos é mais uma ideia de como um jogo completo poderia ficar. Com o usuário podendo criar e editar arquivos de texto, encriptando ou desincriptando arquivos...
// Não coloquei essas funcionalidades por serem muito mais complexas e avançadas para meu conhecimento atual.
//Gera os arquivos TXT que serão usados pelo programa.
void criarTXT(const string& nomeArquivo, const vector<string> arrayTextos) { //Recebe como parâmetros: nome do arquivo, um vetor com os textos e o tamnho total do vetor.
    ofstream arquivoTexto(nomeArquivo); //Cria um arquivo de texto.

    if (arquivoTexto.is_open()) { //Checa se ele está aberto.
        for (string linha : arrayTextos) { //Caso sim, ele lê linha por linha do vetor...
            arquivoTexto << linha; //E transfere para o arquivo de texto
        }
        arquivoTexto.close(); //Por fim fecha o arquivo.
    }
    else { //Caso a criação de arquivo falhe.
        cout << "FALHA AO CRIAR ARQUIVO " << nomeArquivo << "! POR FAVOR, CONTATE O CRIADOR DO PROGRAMA!!!\n\n"; //Ele avisa o usuário.
    }
    
}

//Gera arquivos .bin para o jogo
void criarBIN(const string& nomeArquivo, const vector<string> arrayTextos, int arrayTamanho) { //Recebe o nome do arquivo, o vetor de textos e o tamanho total do array.
    ofstream arquivoBin(nomeArquivo, ios::binary); //Cria um arquivo .bin com o nome passado.
    if (!arquivoBin.is_open()) { //Checa se ele está aberto, caso não ele joga um erro.
        throw runtime_error("Erro ao abrir arquivo para escrita");
    }

    //Escreve o tamanho do vetor em binário
    arquivoBin.write(reinterpret_cast<const char*>(&arrayTamanho), sizeof(arrayTamanho));
    if (arquivoBin.fail()) { //Avisa se falhar.
        throw runtime_error("Erro ao gravar tamanho do array");
    }

    for (const auto& linha : arrayTextos) { //Lê linha por linha do array de textos.
        size_t tamanhoStr = linha.size(); //Indentifica o tamanho da linha.
        arquivoBin.write(reinterpret_cast<const char*>(&tamanhoStr), sizeof(tamanhoStr)); //Transforma o tamanho da linha em binário.
        arquivoBin.write(linha.data(), tamanhoStr); //Transforma todo o texto da linha em binário e passa para o .bin
    }

    arquivoBin.close(); //Fecha o arquivo.
    //cout << "Dados gravados com sucesso." << endl;
}

//Transforma arquivo .bin em arquivos .txt
//Essa função será usada futuramente em outra função que foi criada para desencriptar arquivos dentro do jogo.
void binParaTxt(const string& nomeArquivo) { //Recebe apenas o nome do arquivo.
    ifstream arquivoBin(nomeArquivo, ios::binary); //Abre o arquivo

    if (!arquivoBin.is_open()) { //Checa se está realmente aberto, caso não, joga um erro
        throw runtime_error("Erro ao abrir arquivo para leitura");
    }

    int arrayTamanho; //Lê o tamanho do vetor
    arquivoBin.read(reinterpret_cast<char*>(&arrayTamanho), sizeof(arrayTamanho));
    if (arquivoBin.fail()) { //Avisa se falhar
        throw runtime_error("Erro ao ler o tamanho do array");
    }

    vector<string> tempStringArray; //Criação de um vetor temporário para a String


    for (int i = 0; i < arrayTamanho; ++i) { //Lê cada linha binária da futura string
        size_t tamanhoStr;//Pega o tamanho dela
        arquivoBin.read(reinterpret_cast<char*>(&tamanhoStr), sizeof(tamanhoStr));

        string linha(tamanhoStr, '\0'); //Transforma em texto novamente.
        arquivoBin.read(&linha[0], tamanhoStr);

        tempStringArray.push_back(move(linha)); //Adiciona ao vetor temporário
    }

    //Cria um arquivo de texto com o texto temporário
    criarTXT("textoDensecriptado.txt", tempStringArray);

    arquivoBin.close(); //Fecha o arquivo.bin
}

//<--------------------------- Funções de manipulação de texto --------------------------->
//Essa função é apenas um cosmético que aprendi a fazer, ela imprime letra por letra de uma string de uma forma mais lenta, simulando o digitar de um teclado.
void letraPorLetra(string texto, int delay) { //Recebe um texto e o delay de digitação
    for (int i = 0; i != texto.size(); i++) { //Enquanto i não for maior que o tamanho do texto, aumente i em 1
        cout << texto[i]; //Imprime a letra na posição i
        Sleep(delay); //apresenta um delay
    }
}

//Essa função exibe um arquivo TXT na tela.
void exibirTXT(const string nomeArquivo) { //recebe o nome do arquivo.
    ifstream arquivoTexto(nomeArquivo); //abre o arquivo com o nome passado.
    if (arquivoTexto.is_open()) { //Checa se o arquivo está aberto
        string linha; //caso sinha, pega linha, 
        while (getline(arquivoTexto, linha)) {
            letraPorLetra(linha, 25); //imprime letra por letra e da um espaço no final
            cout << endl;
        }
        arquivoTexto.close();// fecha o arquivo
    }
    else { //caso falhe, mostra esse erro.
        cout << "ERRO AO ABRIR O ARQUIVO " << nomeArquivo << " ! INFORME O CRIADOR DO SOFTWARE\n\n";
    }
}

void alinharTextoCentro(string text) { // --------------------------------------> Código pego da internet para centralizar textos. -> Desta forma, certos textos que deveriam ter destaque ficarão ao centro
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Identifica o console
    PCONSOLE_SCREEN_BUFFER_INFO lpScreenInfo = new CONSOLE_SCREEN_BUFFER_INFO(); // Ponteiro para as informações da tela apontando para uma informação temporal da tela.
    GetConsoleScreenBufferInfo(hConsole, lpScreenInfo); // Salva as informações da tela no ponteiro lpScreenInfo.
    COORD NewSBSize = lpScreenInfo->dwSize; // Obtém o tamanho da tela
    if (NewSBSize.X > text.size()) {
        int newpos = ((NewSBSize.X - text.size()) / 2); // Calcule o número de espaços para centralizar o texto específico.
        for (int i = 0; i < newpos; i++) std::cout << " "; // Imprime os espaços
    }

    cout << text << endl;

    //Fonte: https://stackoverflow.com/questions/45582095/c-how-can-i-center-text-output
}

//Tela final do programa
void telaFinal() {
    system("cls");
    cout << endl;
    alinharTextoCentro("Um computador antigo... [Versão Experimental]");
    cout << endl;
    letraPorLetra("Por: Ricardo Hideki Tsugikava Junior\n", 25);
    cout << endl;
    exit(0);
}

//-------------------------> Telas Inicias <-------------------------
//Tela sobre do programa com créditos e matérias.
void telaSobre() { 
    system("cls");
    alinharTextoCentro("Sobre");
    cout << endl;
    cout << "Criador: Ricardo Hideki Tsugikava Junior\n";
    cout << "Jogo desenvolvido para atividade de projeto autoral da matéria de Algoritmos e Programação II\n";
    cout << "Profe. Berssa";
    cout << endl;             

    system("pause");
}

//Tela inicial do programa.
//Retorna um valor para a função main que decidirá se o programa fecha ou não.
int telaInicial() {
    int escolha = 3; //escolha com valor aleatório para não cancelar o while abaixo

    while (escolha != 0) { //Enquanto não for 0
        cout << endl;
        alinharTextoCentro("Um computador antigo..."); //Imprime o título do jogo centralizado
        cout << endl;
        cout << "1 - Iniciar\n"; //Opções
        cout << "2 - Sobre\n";
        cout << "0 - Sair\n";
        cout << "> ";

        cin >> escolha; //Recebe um inteiro como escolha
        cin.ignore(); //Limpa buffer

        switch (escolha) { //switch sobre a escolha do usuário
        case 1:
            return 1; //retorna 1 = prosseguir com o programa
        case 2:
            telaSobre(); //mostra a tela sobre do programa
            system("cls"); //limpa a tela
            break;
        case 0:
            return 0; //retorna 0 = fechar o programa
        default:
            system("cls"); //limpa a tela e "reseta"
            break;
        }
    }
}

//Uma pequena contextualização sobre do que se trata o jogo.
void introducao() {
    system("cls");
    cout << endl;
    alinharTextoCentro("Você encontrou um computador antigo.");
    cout << endl;
    cout << "Em meio as suas explorações urbanas você encontra um computador antigo, similar a um Commodore, no sótão de uma casa abandonada. Você resolve leva-lo para casa para ver se está funcionado e, talvez, conseguir algum dinheiro sobre isso...\n";
    cout << "É recomendável jogar este jogo com a janela maximizada.";
    cout << endl;
    system("pause");
}

//Animação que simula um computador ligando -> Decidi fazer essa animação para parecer que o usuário está realmente ligando um computador antigo
void computadorLigando() {
    alinharTextoCentro("   ./%.");
    Sleep(delaySimulation);
    alinharTextoCentro("  .%%%.");
    Sleep(delaySimulation);
    alinharTextoCentro(" .%%%%.");
    Sleep(delaySimulation);
    alinharTextoCentro(" #%%%%#");
    Sleep(delaySimulation);
    alinharTextoCentro(".#%%%%%"); 
    Sleep(delaySimulation);                                       //Desenha linha por linha de um desenho ascii de uma banana com delays
    alinharTextoCentro("   .#%%%%%%.");
    Sleep(delaySimulation);
    alinharTextoCentro("        .##%%%%%%%.");
    Sleep(delaySimulation);
    alinharTextoCentro("              .###%%%%%%%%%%%%");
    Sleep(delaySimulation);
    alinharTextoCentro("                .#####%%%%%");
    Sleep(delaySimulation);

    cout << endl;
    Sleep(delaySimulation);
  
    alinharTextoCentro("Banana Computer Company"); //Imprime a marca da "placa-mãe"
    Sleep(delaySimulation);

    cout << "Favorite Modular BIOS v4.51, An Goiaba Ally\n"; //Imprime modelo
    cout << "Copyright (C) 1984-97, Banana Computer Company.\n"; //Copyritht
    Sleep(delaySimulation);
    cout << endl;
    cout << "(29XWESQ23) Acetel b0509BF PCIset (TM)\n"; //Modelo do processador
    Sleep(delaySimulation);
    cout << endl;
    cout << "Mente-S CPU at 25MHz\n"; //Modelo da CPU
    cout << "Memory at 8000K OK\n"; //Modelo da RAM
    Sleep(delaySimulation);
    cout << endl;
    cout << "Favorite Plug and Play BIOS Extesion v9.9Z\n"; //Modelo da BIOS
    cout << "Copyright (C) 1997, Banana Computer Company.\n"; //Copyright 2
    Sleep(maisDelaySimulation);
    cout << "    Detecting IDE Primary Master ... PCCoolHD\n"; //modelos e modelos...
    Sleep(maisDelaySimulation);
    cout << "    Detecting IDE Primary Slave ... PCCemHD\n";
    Sleep(maisDelaySimulation);
    cout << "    Detecting IDE Primary Master ... none\n";
    Sleep(maisDelaySimulation);
    cout << "    Detecting IDE Primary Slave ... none\n";
    Sleep(maisDelaySimulation);
    cout << "    Detecting Location ... Brazil\n";
    Sleep(maisDelaySimulation);
    cout << "    Detecting Language ... portuguese\n";
    Sleep(maisDelaySimulation);

    cout << endl;
                 
    alinharTextoCentro("Olá Berssanete...\n"); //pequeno susto.
    Sleep(delaySimulation);
    system("cls"); //limpa tela.
}

//Essa animação irá tocar no final do jogo como um pequeno susto, onde a criatura se mostra aproximando-se do usuário. Ela utiliza uma ação recursiva para se aproximar e da um encerramento ao programa.
void animacaoFinal(int posicaoInicial, int posicaoFinal, int posicaoFinalUsuario) {
    system("cls");

    alinharTextoCentro("Estou indo atrás de você :)"); //Alinha esse texto na tela.

    cout << endl;

    for (int i = 0; i < posicaoInicial; i++) {
        cout << " "; // Cria espaços antes do boneco
    }
    cout << " O\n"; // Cabeça do boneco

    for (int i = 0; i < posicaoInicial; i++) { //Cria espaços antes do corpo e braços do bonecto
        cout << " ";
    }
    cout << "/|\\"; // Corpo e braços

    for (int i = 0; i < posicaoFinalUsuario; i++) { //Cria espaço do corpo do boneco e o usuário "você".
        cout << " ";
    }
    cout << "Você\n";

    for (int i = 0; i < posicaoInicial; i++) { //Cria espaço antes das pernas do boneco
        cout << " ";
    }
    cout << "/ \\\n"; // Pernas

    // Aguarda antes de atualizar a posição
    Sleep(500);

    // Move o boneco para a próxima posição ou reinicia
    if (posicaoInicial < posicaoFinal) {
        animacaoFinal(posicaoInicial + 1, posicaoFinal, posicaoFinalUsuario - 1); // Chamada recursiva que adiciona +1 para a posição do boneco e remove -1 para a posição do você, dando uma ilusão de que o boneco está se aproximando do "você".
    }

    telaFinal(); //executa a tela final.
}

//-------------------------> Ambiente Principal <-------------------------
//----------------> comandos <-----------------
//Abaixo há diversas funções que simulam comando de terminal.

//ao escrever "ajuda" no terminal essa lista aparecerá para indicar quais comandos o usuário pode usar. -> Utilizei disso para localizar o jogador sobre o que ele pode fazer.
void imprimirAjuda() {
    cout << endl;
    alinharTextoCentro("Lista de Comandos");
    cout << "abrir {nome do Arquivo/Programa}     Abre um arquivo ou programa indicado pelo usuário.\n";
    cout << "ajuda                                Abre a lista de comandos do computador\n";
    cout << "ld                                   LD é uma abreviação para \"Listar Diretório\", e serve para listar todos os arquivos.\n";
    cout << "limpar                               Limpa sua tela.\n";
    cout << "md                                   MD é uma abreviação para \"Mudar de Diretório\", e serve para mudar a pasta atual.\n";
    cout << "sair                                 Desliga o programa. (Você faria isso mesmo?)\n";
    cout << endl;
}

//ao escrever "ld" (listar diretórios" o programa imprime todos os arquivos/pastas que estão presentes no diretório atual do usuário.
void listarDiretorios(int diretorioAtual) { //recebe uma int que representa o diretorio atual do jogador.
    cout << endl;
    switch (diretorioAtual) { //switch para identificar o diretório e imprimir a lista certa de cada.
    case 0: // diretório inicial
        cout << "28/12/1999  22:14  0500  PROBLEMAS.txt\n"; //Arquivo de texto que introduz o jogador a história principal do jogo
        cout << "05/04/1999  13:24  6249  decriptor.exe\n"; //Programa para descriptografar um arquivo que verá mais para frente.
        cout << "15/01/1999  08:48  9434  \\testes\n"; //Uma pasta chamada testes
        break;
    case 1: //diretório testes
        cout << "15/01/1999  14:00  1200  enigma.txt\n"; //Um arquivo que mostra um enigma que serve mais como uma pista falsa.
        cout << "02/02/1999  18:24  2100  ola.cript\n"; //um arquivo criptografado que pode ser vizualizá-lo ao utilizar o programa decriptor.exe nele.
    default:
        break;
    }
}

//O aplicativo que simula o bloco de notas no computador. Ele ainda não possui uma forma de editar textos, mas ainda sim ajuda o jogador para vizualizá-los.
void bananaNotepad(const string textos) { //recebe o texto que o usuário deseja ler como parâmetro
    system("cls");
    alinharTextoCentro("Banana Notepad [1.23.9]"); //Imprime o cabeçalho do programa
    cout << endl;
    exibirTXT(textos); //Executa a função exibirTXT para imprimir o texto salvo no computador.
    system("pause");
    system("cls");
}

//O programa que consegue descriptograr arquivos dentro do jogo. Ele basicamente utiliza aquele tradutor de binário para texto como se simulasse um processo de resolução de criptografia.
void decriptor() {

    int opcao; //declara um int para opções
    string nomeArquivo; //declara uma string para identificar o nome do arquivo
    alinharTextoCentro("Decriptor.exe by DCI"); //Alinha o nome do programa no centro.
    cout << endl; 
    cout << "1 - Encriptar" << endl; //Imprime as opções
    cout << "2 - Desencriptar" << endl;
    cout << "3 - Sair" << endl;

    while (true) {
        cout << "Escolha uma opção: "; //pede ao usuário uma escolha
        cin >> opcao; //lê a escolha
        cin.clear(); //limpa o buffer
        cin.ignore();

        switch (opcao) { //switch para a escolha
        case 1: //Essa função não está disponível por falta de tempo na produção, porém, ela não agregaria nada para o jogo.
            cout << "Função indisponV̵̢̨̢̢̙̫̬̭̺͖̱̣̓̽̆́̀͜Á̵̢̧̲͇͍͓̖͖͉̙̞͙̖͚̐͛̓̐̏͂͋͑̍̈͑̌̚ ̷̩̂̿̆͠͝Ę̵͎̰͈̥̯͗̒͋̃̀́̔̓̚͠M̴̡̹̫̙̗̗̜̪̭͊̎̊͆̋͊̈̾̅̓͆B̵̡̘̯̠̼̭͕͙̝̭̣͗̾͗̏͝͠ͅO̶̙͓̖̍̐͆͆Ŗ̸̝͓̀͆̒̌̈́̚Ă̶̢̨͚" << endl; //O texto corrompido é proposital, caso você leia apenas a letra está escrito "Vá embora".
            break;
        case 2: //caso o jogador queria desincriptar um arquivo...
            cout << endl;
            cout << "Insira o nome do arquivo que deseja Desencriptar: "; //ele pede o nome do arquivo.
            getline(cin, nomeArquivo); //lê o nome do arquivo.
            if (nomeArquivo == "ola.cript") { //identifica se é o arquivo correto a ser descriptografado
                binParaTxt("ola.bin"); //transforma o arquivo bin em texto para leitura.
                cout << endl;
                cout << "Arquivo desincriptado com sucesso!\n"; //avisa o usuário sobre
                cout << endl;
                system("pause"); //pausa o sistema
                bananaNotepad("textoDensecriptado.txt"); //abre o arquivo descriptografado no bananaNotepad.
                animacaoFinal(0, 20, 20); //Inicia o fim do jogo -> O fim do jogo está nessa função por, também, falta de tempo. Em uma versão futura do jogo procura aumentar o número de enigmas e entrada do investigador.
                return;
            }
            else { //Caso o usuário escreva qualquer outro arquivo ele da como inválido.
                cout << endl;
                cout << "diretório inválido!" << endl;
                cout << endl;
            };
            break;
        case 3:
            system("cls"); //limpa a tela e sai do programa
            return;
        default:
            cout << endl;
            cout << "Insira um valor válido!" << endl << endl; //Caso o usuário escolha outro número que não seja 1, 2 ou 3 ele da essa mensagem.
        }
    }

}

//Função abrir, ela serve para abrir arquivos e programas que estão no diretório que o jogador se encontra. Outra maneira de tentar emular esse sistema de terminais.
void abrir(string arquivo, int diretorioAtual) {  // <---------------------------------- Terminar esta parte depois.
    string arquivosDCI[2] = {"PROBLEMAS.txt", "decriptor.exe"}; //array de nomes de arquivos do diretório inicial.
    string arquivosTestes[2] = {"enigma.txt", "ola.cript"}; //array de nomes de arquivos da pasta \testes.

    switch (diretorioAtual) { //Switch para identificar qual diretório o jogador está para abertura de arquivos e programas.
    case 0:  //Se estiver o diretório inicial ele pode abrir os programas abaixo
        if (arquivosDCI[0] == arquivo) { //Se o jogador digitar "abrir PROBLEMAS.txt" ele abrir o arquivo de texto com mesmo nome.
            bananaNotepad(problemas.arquivoNome);
        }
        else if (arquivosDCI[1] == arquivo) { //Se o jogador digitar "abrir decriptor.exe" ele executa o programa decriptor
            system("cls");
            decriptor();
        }
        else { //Caso ele digite qualquer outra coisa ele recebe essa mensagem abaixo.
            cout << "Nenhum arquivo encontrado com esse nome!\n";
        }
        break;
    case 1: //Caso o jogador esteja no diretório "\pastas"
        if (arquivosTestes[0] == arquivo) { //se digitar "abrir enigma.txt"
            bananaNotepad(enigma.arquivoNome); //Abrirá o arquivo de texto de mesmo nome
        }
        else if (arquivosTestes[1] == arquivo) { //"abrir ola.cript" resulta em um erro, pois é um arquivo criptografado.
            cout << endl;
            cout << "Erro ao abrir o arquivo! Extensão não identificada.\n";
            cout << endl;
        }
        else {//Caso ele digite qualquer outra coisa ele recebe essa mensagem abaixo.
            cout << "Nenhum arquivo encontrado com esse nome!\n";
        };
        break;
    default:
        cout << "Nenhum arquivo encontrado com esse nome!\n";
        break;
    }
}

void mudarDiretorio(int& diretorioAtual, string parametro) { //Função para mudar entre pastas dentro do terminal.
    if (parametro == "\\testes") {//se o usuário digitar "md \testes" ele é direcionado para um nome diretório, retornado o valor do novo diretório.
        diretorioAtual = 1;
    }
    else if (parametro == "..") {//Se o usuário escrever "md .." ele retorna para o diretório inicial -> Não consegui pensar em uma forma sem ser tão direto, isso provavelmente quebraria se tivesse mais diretórios.
        diretorioAtual = 0;
    }
    else { //Se o jogador digitar um diretório que não existe ou que já está, ele receberá este erro.
        cout << endl;
        cout << "Você já está ou não há diretórios com este nome!\n";
        cout << endl;
    };
}

//Essa função imprime e gerencia todo o ambiente do terminal, ela não está na função main pois a mesma dita como o jogo é feito e não seu terminal.
void ambienteComputador() {
    string escolha; //String que guarda o comando escrito pelo usuário.
    string opcoes[7] = {"ajuda", "ld", "md", "abrir", "sair", "limpar", ""}; //Array de comandos que o usuário pode escrever.
    string diretorios[2] = {"A:\\DCI", "A:\\DCI\\testes"}; //Array de diretórios

    int diretorioAtual = 0;

    alinharTextoCentro("Banana OS [Versão 10.00.123.4124A]\n"); //Título do Sistema
    alinharTextoCentro("(c) Banana Computer Company. Todos os direitos reservados\n");

    while (true) {
        string comando, parametro; //cria strings para receber o comando e o parâmetro do comando.

        cout << diretorios[diretorioAtual] << "> ";
        getline(cin, escolha); //Pede para o usuário um comando para o terminal.

        istringstream stream(escolha);  //divide a escolha em várias partes a partir dos espaços

        stream >> comando; //recebe o comando do usuário
        stream >> parametro; //recebe o parâmetro que ele pasosu.

        auto ptr = find(opcoes, opcoes + 6, comando); //Cria um ponteiro para o array de opções em relação ao que o usuário escolheu.
        int idx = ptr - opcoes; //Subtrai o ponteiro do array opções para pegar exatamente o indíce que está localizado.

        switch (idx) { //Switch para cada opção de acordo com o índice.
        case 0: //Menu Ajuda
            imprimirAjuda();
            break;
        case 1: //Comando de listar diretórios
            listarDiretorios(diretorioAtual);
            cout << endl;
            break;
        case 2: //Comando de mudar de diretório
            mudarDiretorio(diretorioAtual, parametro);
            cout << endl;
            break;
        case 3: //Comando para abrir um arquivo
            abrir(parametro, diretorioAtual);
            cout << endl;
            break;
        case 4: //Comando para sair do computador
            cout << endl;
            cout << "É uma pena que tenhamos que nos despedir tão cedo :(\n";
            return;
        case 5: //Comando limpar a tela
            system("cls");
            break;
        default: //Caso nenhum comando seja identificado.
            cout << endl;
            cout << "Comando inválido, tente utilizar o comando \"ajuda\" para mais informações!\n";
            cout << endl;
            break;
        }

    }
  
}

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8"); //Seta a localização como pt_br utf-8

    //<------------- Cria os structs sobre os arquivos para melhor organização ------------->
    problemas.arquivoNome = "PROBLEMAS.txt"; 
    problemas.textoArrayTamanho = 6;
    problemas.texto = textosProblemas;

    enigma.arquivoNome = "enigma.txt";
    enigma.textoArrayTamanho = 2;
    enigma.texto = textoEnigma;

    ola.arquivoNome = "ola.bin";
    ola.textoArrayTamanho = 2;
    ola.texto = textoEncriptado;

    //<------------ Cria os arquivos necessários para o funcionamento do jogo ------------->
    criarTXT(problemas.arquivoNome, problemas.texto);
    criarTXT(enigma.arquivoNome, enigma.texto);
    criarBIN(ola.arquivoNome, ola.texto, ola.textoArrayTamanho);

    int escolha;

    escolha = telaInicial(); //Apresenta a tela inicial

    if (escolha == 0) { //Se a escolha do jogador for 0, finaliza o jogo
        return 0;
    }

    introducao(); //Apresenta a introdução do jogo

    system("cls"); 

    computadorLigando(); //Mostra a animação do computador ligando

    ambienteComputador(); //Direciona o jogador para o sistema utilizado.
}