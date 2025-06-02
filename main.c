#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

#define MAX_RELATOS 100
#define RAIO_MAXIMO 10.0 // 10 km
#define EARTH_RADIUS 6371.0 // Raio da Terra em km

typedef struct {
    char nome[100];
    char documento[20];
    char email[100];
    char telefone[15];
    double latitude;
    double longitude;
} Relator;

typedef enum {
    ENCHENTE,
    INCENDIO,
    DESLIZAMENTO,
    OUTRO
} TipoCatastrofe;

typedef struct {
    int id;
    TipoCatastrofe tipo;
    char descricao[500];
    char data[11]; // DD/MM/AAAA
    char hora[6];  // HH:MM
    double latitude;
    double longitude;
    Relator relator;
} Relato;

Relato relatos[MAX_RELATOS];
int totalRelatos = 0;

// Estrutura para ponto central
typedef struct {
    double latitude;
    double longitude;
} PontoCentral;

PontoCentral centro = {0.0, 0.0}; // Inicializado com valores padrão

// Função para calcular distância usando fórmula de Haversine
double calcular_distancia(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    
    lat1 = lat1 * M_PI / 180.0;
    lat2 = lat2 * M_PI / 180.0;
    
    double a = sin(dLat/2) * sin(dLat/2) +
               sin(dLon/2) * sin(dLon/2) * cos(lat1) * cos(lat2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    
    return EARTH_RADIUS * c;
}

// Valida se um relato está dentro do raio permitido
int validar_localizacao(double lat, double lon) {
    double distancia = calcular_distancia(lat, lon, centro.latitude, centro.longitude);
    return (distancia <= RAIO_MAXIMO);
}

// Valida formato de email simples
int validar_email(const char *email) {
    int tem_arroba = 0, tem_ponto = 0;
    for (int i = 0; email[i]; i++) {
        if (email[i] == '@') tem_arroba = 1;
        if (email[i] == '.' && tem_arroba) tem_ponto = 1;
    }
    return (tem_arroba && tem_ponto);
}

// Valida formato de data (DD/MM/AAAA)
int validar_data(const char *data) {
    if (strlen(data) != 10) return 0;
    if (data[2] != '/' || data[5] != '/') return 0;
    
    int dia = atoi(data);
    int mes = atoi(data + 3);
    int ano = atoi(data + 6);
    
    if (dia < 1 || dia > 31) return 0;
    if (mes < 1 || mes > 12) return 0;
    if (ano < 1900 || ano > 2100) return 0;
    
    return 1;
}

// Valida formato de hora (HH:MM)
int validar_hora(const char *hora) {
    if (strlen(hora) != 5) return 0;
    if (hora[2] != ':') return 0;
    
    int hh = atoi(hora);
    int mm = atoi(hora + 3);
    
    if (hh < 0 || hh > 23) return 0;
    if (mm < 0 || mm > 59) return 0;
    
    return 1;
}

// Cadastro de relator com validações
Relator cadastrar_relator() {
    Relator r;
    printf("\n--- Cadastro de Relator ---\n");
    
    do {
        printf("Nome completo: ");
        fgets(r.nome, sizeof(r.nome), stdin);
        r.nome[strcspn(r.nome, "\n")] = 0;
    } while (strlen(r.nome) == 0);
    
    do {
        printf("Documento (CPF/RG): ");
        fgets(r.documento, sizeof(r.documento), stdin);
        r.documento[strcspn(r.documento, "\n")] = 0;
    } while (strlen(r.documento) == 0);
    
    do {
        printf("Email: ");
        fgets(r.email, sizeof(r.email), stdin);
        r.email[strcspn(r.email, "\n")] = 0;
    } while (!validar_email(r.email));
    
    do {
        printf("Telefone: ");
        fgets(r.telefone, sizeof(r.telefone), stdin);
        r.telefone[strcspn(r.telefone, "\n")] = 0;
    } while (strlen(r.telefone) < 8);
    
    do {
        printf("Localizacao (latitude longitude): ");
        scanf("%lf %lf", &r.latitude, &r.longitude);
        while (getchar() != '\n'); // Limpa buffer
    } while (r.latitude < -90 || r.latitude > 90 || 
             r.longitude < -180 || r.longitude > 180);
    
    return r;
}

// Cadastro de relato com validações
void cadastrar_relato() {
    if (totalRelatos >= MAX_RELATOS) {
        printf("Limite maximo de relatos atingido!\n");
        return;
    }
    
    Relato novo;
    novo.id = totalRelatos + 1;
    
    // Cadastra relator
    novo.relator = cadastrar_relator();
    
    // Seleciona tipo de catástrofe
    printf("\n--- Cadastro de Relato ---\n");
    printf("Tipo de catastrofe:\n");
    printf("0 - Enchente\n1 - Incendio\n2 - Deslizamento\n3 - Outro\n");
    
    int tipo;
    do {
        printf("Escolha (0-3): ");
        scanf("%d", &tipo);
        while (getchar() != '\n'); // Limpa buffer
    } while (tipo < 0 || tipo > 3);
    
    novo.tipo = (TipoCatastrofe)tipo;
    
    do {
        printf("Descricao: ");
        fgets(novo.descricao, sizeof(novo.descricao), stdin);
        novo.descricao[strcspn(novo.descricao, "\n")] = 0;
    } while (strlen(novo.descricao) == 0);
    
    do {
        printf("Data (DD/MM/AAAA): ");
        fgets(novo.data, sizeof(novo.data), stdin);
        novo.data[strcspn(novo.data, "\n")] = 0;
    } while (!validar_data(novo.data));
    
    do {
        printf("Hora (HH:MM): ");
        fgets(novo.hora, sizeof(novo.hora), stdin);
        novo.hora[strcspn(novo.hora, "\n")] = 0;
    } while (!validar_hora(novo.hora));
    
    do {
        printf("Localizacao do evento (latitude longitude): ");
        scanf("%lf %lf", &novo.latitude, &novo.longitude);
        while (getchar() != '\n'); // Limpa buffer
        
        if (!validar_localizacao(novo.latitude, novo.longitude)) {
            printf("Localizacao fora do raio de 10km do ponto central!\n");
        }
    } while (!validar_localizacao(novo.latitude, novo.longitude));
    
    relatos[totalRelatos++] = novo;
    printf("Relato cadastrado com sucesso! ID: %d\n", novo.id);
}

// Função auxiliar para converter data para formato comparável
int data_para_int(const char *data) {
    int dia, mes, ano;
    sscanf(data, "%d/%d/%d", &dia, &mes, &ano);
    return ano * 10000 + mes * 100 + dia;
}

// Busca relatos por tipo
void buscar_por_tipo() {
    printf("\nTipos disponiveis:\n");
    printf("0 - Enchente\n1 - Incendio\n2 - Deslizamento\n3 - Outro\n");
    
    int tipo;
    do {
        printf("Digite o tipo: ");
        scanf("%d", &tipo);
        while (getchar() != '\n');
    } while (tipo < 0 || tipo > 3);
    
    printf("\nRelatos encontrados:\n");
    for (int i = 0; i < totalRelatos; i++) {
        if (relatos[i].tipo == tipo) {
            printf("ID: %d | Data: %s | Local: (%.6f, %.6f)\n", 
                   relatos[i].id, relatos[i].data, 
                   relatos[i].latitude, relatos[i].longitude);
        }
    }
}

// Busca relatos por localização
void buscar_por_localizacao() {
    double lat, lon, distancia;
    printf("\nDigite as coordenadas (latitude longitude): ");
    scanf("%lf %lf", &lat, &lon);
    while (getchar() != '\n');
    
    printf("Digite o raio maximo em km: ");
    scanf("%lf", &distancia);
    while (getchar() != '\n');
    
    printf("\nRelatos encontrados:\n");
    for (int i = 0; i < totalRelatos; i++) {
        double d = calcular_distancia(lat, lon, 
                                     relatos[i].latitude, 
                                     relatos[i].longitude);
        if (d <= distancia) {
            printf("ID: %d | Distancia: %.2f km | Data: %s\n", 
                   relatos[i].id, d, relatos[i].data);
        }
    }
}

// Busca relatos por período
void buscar_por_periodo() {
    char inicio[11], fim[11];
    
    do {
        printf("\nData inicial (DD/MM/AAAA): ");
        fgets(inicio, sizeof(inicio), stdin);
        inicio[strcspn(inicio, "\n")] = 0;
    } while (!validar_data(inicio));
    
    do {
        printf("Data final (DD/MM/AAAA): ");
        fgets(fim, sizeof(fim), stdin);
        fim[strcspn(fim, "\n")] = 0;
    } while (!validar_data(fim));
    
    int data_inicio = data_para_int(inicio);
    int data_fim = data_para_int(fim);
    
    printf("\nRelatos encontrados:\n");
    for (int i = 0; i < totalRelatos; i++) {
        int data_relato = data_para_int(relatos[i].data);
        if (data_relato >= data_inicio && data_relato <= data_fim) {
            printf("ID: %d | Data: %s | Tipo: %d\n", 
                   relatos[i].id, relatos[i].data, relatos[i].tipo);
        }
    }
}

// Menu principal
void menu_principal() {
    int opcao;
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Cadastrar novo relato\n");
        printf("2. Buscar por tipo de catastrofe\n");
        printf("3. Buscar por localizacao\n");
        printf("4. Buscar por periodo\n");
        printf("5. Listar todos os relatos\n");
        printf("6. Definir ponto central\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        while (getchar() != '\n'); // Limpa buffer
        
        switch (opcao) {
            case 1:
                cadastrar_relato();
                break;
            case 2:
                buscar_por_tipo();
                break;
            case 3:
                buscar_por_localizacao();
                break;
            case 4:
                buscar_por_periodo();
                break;
            case 5:
                printf("\nTodos os relatos:\n");
                for (int i = 0; i < totalRelatos; i++) {
                    printf("ID: %d | Tipo: %d | Data: %s | Local: (%.6f, %.6f)\n",
                           relatos[i].id, relatos[i].tipo, relatos[i].data,
                           relatos[i].latitude, relatos[i].longitude);
                }
                break;
            case 6:
                printf("\nDefinir ponto central:\n");
                printf("Latitude: ");
                scanf("%lf", &centro.latitude);
                printf("Longitude: ");
                scanf("%lf", &centro.longitude);
                while (getchar() != '\n');
                printf("Ponto central atualizado!\n");
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}

int main() {
    printf("=== Sistema de Cadastro de Catastrofes Naturais ===\n");
    
    // Configura ponto central padrão (exemplo: São Paulo)
    centro.latitude = -23.5505;
    centro.longitude = -46.6333;
    printf("Ponto central padrao definido: (-23.5505, -46.6333)\n");
    
    menu_principal();
    return 0;
}