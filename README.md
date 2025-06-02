 GeoRelato – Sistema de Cadastro de Catástrofes Naturais
GeoRelato é uma CLI em C que permite o registro e consulta de relatos sobre catástrofes naturais (enchentes, incêndios, deslizamentos etc.), ocorridas em um raio de até 10 km a partir de um ponto central definido pelo usuário. Ideal para projetos acadêmicos de Estrutura de Dados e Banco de Dados.

🎯 Visão Geral
Idioma: C

Interface: Linha de Comando (CLI)

Objetivo:

Registrar dados completos do relator (nome, documento, e-mail, telefone, localização).

Cadastrar relatos de catástrofes (tipo, descrição, data, hora, coordenadas).

Validar se o evento está dentro de um raio de 10 km do ponto central (fórmula de Haversine).

Permitir buscas por tipo, localização e período.

Listar todos os relatos já cadastrados.

🚀 Funcionalidades Principais
Cadastro de Relator

Nome completo

Documento (CPF/RG)

E-mail (validação básica de “@” e “.”)

Telefone (mínimo de 8 dígitos)

Localização (latitude e longitude, dentro de limites reais)

Cadastro de Relato

Tipo de catástrofe:

0 – Enchente

1 – Incêndio

2 – Deslizamento

3 – Outro

Descrição (texto livre)

Data (formato DD/MM/AAAA)

Hora (formato HH:MM)

Localização (latitude/longitude), validada pelo raio de 10 km

Validação Geográfica

Utiliza a fórmula de Haversine para calcular a distância entre duas coordenadas e garantir que cada relato esteja dentro de 10 km do ponto central.

Consultas e Listagens

Buscar por tipo de catástrofe

Buscar por localização (dentro de raio personalizado)

Buscar por período (data inicial e final)

Listar todos os relatos cadastrados

Ponto Central Configurável

Ponto inicial padrão: São Paulo (−23.5505, −46.6333)

Usuário pode redefinir o ponto central a qualquer momento via menu

Navegue pelo menu

=== MENU PRINCIPAL ===
1. Cadastrar novo relato
2. Buscar por tipo de catástrofe
3. Buscar por localização
4. Buscar por período
5. Listar todos os relatos
6. Definir ponto central
0. Sair
Escolha:

🧠 Justificativas Técnicas
2. Estrutura de Dados
Array de structs estático 

Simplicidade e eficiência para acesso sequencial

2. Cálculo de Distância (Haversine)
A fórmula de Haversine considera a curvatura da Terra e fornece distância precisa entre duas coordenadas geográficas.

Implementação em calcular_distancia(double lat1, double lon1, double lat2, double lon2), retornando a distância em quilômetros.

3. Validações de Entrada
E-mail: Presença de @ e . após o símbolo

Data (DD/MM/AAAA):

Tamanho exato de 10 caracteres

Barras em posições 3 e 6

Dia entre 1–31, mês entre 1–12, ano entre 1900–2100

Hora (HH:MM):

Tamanho exato de 5 caracteres

Dois dígitos de hora 00–23, dois dígitos de minuto 00–59

Coordenadas:

Latitudes ∈ [−90, +90], longitudes ∈ [−180, +180]

4. Organização do Código
Uso de funções específicas para cada tarefa (ex.: cadastrar_relator(), cadastrar_relato(), buscar_por_tipo(), etc.).

Tratamento de erros de entrada com loops “do…while” até a validação ser satisfeita.

Limpeza de buffer (uso de while (getchar() != '\n')) para evitar leituras incorretas de stdin.
