# ESP32-project

▪	Programação de um microcontrolador ESP32 para tratar o sinal de um sensor de movimento HC-SR501 e notificar um usuário através do gmail em caso de detecção de movimento

▪	Foram utilizadas bibliotecas de comunicação WiFi e protocolo de comunicação via email SMTP

▪	O protótipo foi testado pelo cliente (professor) e posteriormente utilizado como mínimo produto viável para um projeto de incubação de uma startup para o curso de formação de empreendedores na Universidade de Pernambuco. A equipe utilizou o projeto para o desenvolvimento de um sistema de automação de segurança para pets. 


- Funcionamento do programa:
O programa trata o sinal de um sensor PIR de movimento pelo pino de entrada 19, de forma que caso haja detecção de presença, o programa aciona um led
através do pino 21 e envia um e-mail para um usuário com um alerta. Caso não haja detecção de movimento, o led permanece apagado e nenhum alerta é enviado.
