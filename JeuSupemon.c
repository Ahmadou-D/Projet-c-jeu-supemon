#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Structures */

struct Move { // attaque
    char name[20];
    int damage;
    char effect[20];
};

struct Item {  // objet
    char name[20];
    char effect[20];
    int price;
};

struct Supemon {  // monstre avec stats
    int id;
    char name[20];
    int level;
    int experience;
    int HP;
    int maxHP;
    int attack;
    int baseAttack;
    int defense;
    int baseDefense;
    int evasion;
    int baseEvasion;
    int accuracy;
    int baseAccuracy;
    int speed;
    char moves[2][20];
};

struct Player { // joueur, supemons du joueur, inventaire des objets
    char name[20];
    struct Supemon supemons[20];
    int supemonCount;
    int supcoins;
    struct Item items[10];
    int itemCount;
    int selectedSupemonIndex; /* index du supemon actif */
};


/*  Prototypes  */
void OutOfCombat(struct Player *player); // fonctions du jeu avec type et variables
void intoTheWild(struct Player *player);
void shop(struct Player *player);
void supCenter(struct Player *player);
void savegame(struct Player *player);
void loadgame(struct Player *player);
int randomRound(float value);


/*Fonctions */


// Fonctions randomRound, savegame, & loadgame

int randomRound(float value) {     // arrondit aléatoirement à l'entier supérieur ou inférieur
    int floorVal = (int)value;
    float decimal = value - floorVal;
    if (decimal == 0.0f) return floorVal;
    return (rand() % 2 == 0) ? floorVal : floorVal + 1;
}

void savegame(struct Player *player){  // sauvegarder partie
    FILE *file = fopen("sauvegarde.txt", "w");
    if (file == NULL){
        printf("Error: Unable to create save file.\n");
        return;
    }

    /* sauvegarde simple : nom, supcoins, nombre de supemons et leurs noms/HP/level, items */
    fprintf(file, "%s\n", player->name);
    fprintf(file, "%d\n", player->supcoins);
    fprintf(file, "%d\n", player->supemonCount);
    for (int i = 0; i < player->supemonCount; ++i) {
        struct Supemon *s = &player->supemons[i];
        fprintf(file, "%d|%s|%d|%d|%d\n", s->id, s->name, s->level, s->HP, s->maxHP);
    }
    fprintf(file, "%d\n", player->itemCount);
    for (int i = 0; i < player->itemCount; ++i) {
        fprintf(file, "%s|%s|%d\n", player->items[i].name, player->items[i].effect, player->items[i].price);
    }

    fclose(file);
    printf("Game saved successfully in 'sauvegarde.txt'.\n");
}

void loadgame(struct Player *player) {  //charger partie sauvegardee
    FILE *file = fopen("sauvegarde.txt", "r");
    if (file == NULL) {
        printf("No saving found\n");
        return;
    }

    char line[256];
    if (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        strncpy(player->name, line, sizeof(player->name)-1);
        player->name[sizeof(player->name)-1] = '\0';
    } else { fclose(file); return; }

    if (fgets(line, sizeof(line), file)) player->supcoins = atoi(line);
    if (fgets(line, sizeof(line), file)) {
        player->supemonCount = atoi(line);
        if (player->supemonCount < 0) player->supemonCount = 0;
        if (player->supemonCount > 20) player->supemonCount = 20;
    }

    for (int i = 0; i < player->supemonCount; ++i) {
        if (!fgets(line, sizeof(line), file)) break;
        /* format: id|name|level|HP|maxHP */
        int id, level, hp, maxhp;
        char name[64];
        if (sscanf(line, "%d|%63[^|]|%d|%d|%d", &id, name, &level, &hp, &maxhp) == 5) {
            player->supemons[i].id = id;
            strncpy(player->supemons[i].name, name, sizeof(player->supemons[i].name)-1);
            player->supemons[i].level = level;
            player->supemons[i].HP = hp;
            player->supemons[i].maxHP = maxhp;
        }
    }

    if (fgets(line, sizeof(line), file)) {
        player->itemCount = atoi(line);
        if (player->itemCount < 0) player->itemCount = 0;
        if (player->itemCount > 10) player->itemCount = 10;
    }

    for (int i = 0; i < player->itemCount; ++i) {
        if (!fgets(line, sizeof(line), file)) break;
        char iname[64], ieffect[64];
        int price;
        if (sscanf(line, "%63[^|]|%63[^|]|%d", iname, ieffect, &price) == 3) {
            strncpy(player->items[i].name, iname, sizeof(player->items[i].name)-1);
            strncpy(player->items[i].effect, ieffect, sizeof(player->items[i].effect)-1);
            player->items[i].price = price;
        }
    }

    fclose(file);
    printf("Saving loaded. Welcome %s.\n", player->name);
}



// Fonctions supCenter

void supCenter(struct Player *player) {  // HP max pour tous les supemon
    for (int i = 0; i < player->supemonCount; ++i) {
        player->supemons[i].HP = player->supemons[i].maxHP;
    }
    printf("All Supemon are healed!\n");
}



// Fonction shop

void shop(struct Player *player) {  // shop
    struct Item shopItems[] = {
        {"Potion", "HP+5", 100},
        {"Super Potion", "HP+10", 300},
        {"Rare Candy", "Level+1", 700}
    };
    int nbShop = sizeof(shopItems)/sizeof(shopItems[0]);
    int choiceshop;

    printf("\n+---------- SHOP ----------+\n");
    printf("| 1 - Buy                  |\n");
    printf("| 2 - Leave Shop           |\n");
    printf("+--------------------------+\n");
    printf("Your choice: ");

    if (scanf("%d", &choiceshop) != 1) { while (getchar()!='\n'); return; }

    if (choiceshop == 1) {
        printf("Items available :\n");
        for (int i = 0; i < nbShop; i++) {
            printf("%d - %s (%s) : %d Supcoins\n", i + 1, shopItems[i].name, shopItems[i].effect, shopItems[i].price);
        }
        printf("Which item do you want ? (1-%d) : ", nbShop);
        int choice;
        if (scanf("%d", &choice) != 1) { while (getchar()!='\n'); return; }
        if (choice >= 1 && choice <= nbShop && player->supcoins >= shopItems[choice - 1].price) {
            if (player->itemCount < 10) {
                player->items[player->itemCount++] = shopItems[choice - 1];
                player->supcoins -= shopItems[choice - 1].price;
                printf("Purchase succeeded !\n");
            } else {
                printf("Inventory full.\n");
            }
        } else {
            printf("Impossible purchase!\n");
        }
    }
}


// Fonctions IntoTheWild (combat)

void intoTheWild(struct Player *player) {
    /* Choix aléatoire d'un enemy parmi les 3 modèles */
    struct Supemon enemy;
    int r = rand() % 3;

    if (r == 0) {
        enemy.id = 1;
        strcpy(enemy.name, "Supmander");
        enemy.HP = 10;
        enemy.attack = 1;
        enemy.defense = 1;
        enemy.evasion = 1;
        enemy.accuracy = 2;
        enemy.speed = 1;
        strcpy(enemy.moves[0], "Scratch");
        strcpy(enemy.moves[1], "Growl");
    } else if (r == 1) {
        enemy.id = 2;
        strcpy(enemy.name, "Supasaur");
        enemy.HP = 9;
        enemy.attack = 1;
        enemy.defense = 1;
        enemy.evasion = 3;
        enemy.accuracy = 2;
        enemy.speed = 2;
        strcpy(enemy.moves[0], "Pound");
        strcpy(enemy.moves[1], "Foliage");
    } else {
        enemy.id = 3;
        strcpy(enemy.name, "Supirtle");
        enemy.HP = 11;
        enemy.attack = 1;
        enemy.defense = 2;
        enemy.evasion = 2;
        enemy.accuracy = 1;
        enemy.speed = 2;
        strcpy(enemy.moves[0], "Pound");
        strcpy(enemy.moves[1], "Shell");
    }

    /* Ajuste le niveau selon le supemon actif du joueur (si existant) */
    struct Supemon *active = NULL;
    if (player->supemonCount > 0 && player->selectedSupemonIndex >= 0 &&
        player->selectedSupemonIndex < player->supemonCount) {
        active = &player->supemons[player->selectedSupemonIndex];
    } else if (player->supemonCount > 0) {
        player->selectedSupemonIndex = 0;
        active = &player->supemons[0];
    }

    int level = (active != NULL) ? active->level : 1;
    enemy.level = level;
    enemy.maxHP = randomRound(enemy.HP * (1 + 0.3f * (level - 1)));
    enemy.HP = enemy.maxHP;
    enemy.attack = randomRound(enemy.attack * (1 + 0.3f * (level - 1)));
    enemy.defense = randomRound(enemy.defense * (1 + 0.3f * (level - 1)));
    enemy.evasion = randomRound(enemy.evasion * (1 + 0.3f * (level - 1)));
    enemy.accuracy = randomRound(enemy.accuracy * (1 + 0.3f * (level - 1)));
    enemy.speed = randomRound(enemy.speed * (1 + 0.3f * (level - 1)));

    printf("\n%s is here ! Level %d\n", enemy.name, enemy.level);

    if (active == NULL) {
        printf("You have no Supemon to fight with!\n");
        return;
    }

    int itemUsed = 0;
    int playerTurn = (active->speed > enemy.speed) ? 1 : (active->speed == enemy.speed ? rand()%2 : 0);

    while (active->HP > 0 && enemy.HP > 0) {
        if (playerTurn) {

            printf("\nYour turn!\n");

            /* affichage du tableau de stats */
            printf("\n----------------------------------------\n");
            printf("%s (enemy)\n", enemy.name);
            printf("----------------------------------------\n");
            printf("HP: %d/%d         Lvl: %d\n", enemy.HP, enemy.maxHP, enemy.level);
            printf("Atk: %d           Def: %d\n", enemy.attack, enemy.defense);
            printf("Acc: %d           Eva: %d\n", enemy.accuracy, enemy.evasion);
            printf("----------------------------------------\n\n");

            printf("%s (You)\n", active->name);
            printf("----------------------------------------\n");
            printf("HP: %d/%d         Lvl: %d\n", active->HP, active->maxHP, active->level);
            printf("Atk: %d           Def: %d\n", active->attack, active->defense);
            printf("Acc: %d           Eva: %d\n", active->accuracy, active->evasion);
            printf("----------------------------------------\n");

            printf("+---------------------------+\n");
            printf("| What will you do ?        |\n");
            printf("|  1 - Attack               |\n");
            printf("|  2 - Change Supemon       |\n");
            printf("|  3 - Use item             |\n");
            printf("|  4 - Run away             |\n");
            printf("|  5 - Capture              |\n");
            printf("+---------------------------+\n");
            printf("Choice: ");
            int choice;
            if (scanf("%d", &choice) != 1) { while (getchar()!='\n'); choice = 0; }

            switch (choice) {
                case 1: {
                    printf("Choose an attack:\n1- %s\n2- %s\nChoice: ", active->moves[0], active->moves[1]);
                    int moveChoice;
                    if (scanf("%d", &moveChoice) != 1) { while (getchar()!='\n'); moveChoice = 1; }
                    if (moveChoice < 1 || moveChoice > 2) moveChoice = 1;
                    int damage = 0;
                    float hitChance = (float)active->accuracy / (active->accuracy + enemy.evasion) + 0.1f;
                    if ((rand()%100) < (int)(hitChance*100)) {
                        if (strcmp(active->moves[moveChoice - 1], "Scratch") == 0) damage = 3;
                        else if (strcmp(active->moves[moveChoice - 1], "Pound") == 0) damage = 2;
                        else if (strcmp(active->moves[moveChoice - 1], "Growl") == 0) { active->attack += 1; printf("Your attack increased!\n"); break; }
                        else if (strcmp(active->moves[moveChoice - 1], "Foliage") == 0) { active->evasion += 1; printf("Your evasion increased!\n"); break; }
                        else if (strcmp(active->moves[moveChoice - 1], "Shell") == 0) { active->defense += 1; printf("Your defense increased!\n"); break; }

                        int finalDamage = randomRound((float)(active->attack * damage) / (enemy.defense>0?enemy.defense:1));
                        enemy.HP -= finalDamage;
                        if (enemy.HP < 0) enemy.HP = 0;
                        printf("You deal %d damage to %s! (enemy HP: %d/%d)\n", finalDamage, enemy.name, enemy.HP, enemy.maxHP);
                    } else {
                        printf("The attack failed!\n");
                    }
                    break;
                }
                case 2: {
                    if (player->supemonCount <= 1) { printf("No other Supemon to change.\n"); break; }
                    printf("List of your Supemons:\n");
                    for (int i = 0; i < player->supemonCount; i++) {
                        printf("%d - %s (HP: %d/%d)\n", i + 1, player->supemons[i].name, player->supemons[i].HP, player->supemons[i].maxHP);
                    }
                    printf("Choose a Supemon: ");
                    int newIndex;
                    if (scanf("%d", &newIndex) != 1) { while (getchar()!='\n'); break; }
                    if (newIndex >= 1 && newIndex <= player->supemonCount) {
                        player->selectedSupemonIndex = newIndex - 1;
                        active = &player->supemons[player->selectedSupemonIndex];
                        printf("You changed to %s!\n", active->name);
                    }
                    break;
                }
                case 3: {
                    if (itemUsed >= 4) { printf("You have already used 4 items this battle.\n"); break; }
                    if (player->itemCount == 0) { printf("No items.\n"); break; }
                    printf("Inventory:\n");
                    for (int i = 0; i < player->itemCount; i++)
                        printf("%d - %s (%s)\n", i+1, player->items[i].name, player->items[i].effect);
                    printf("Choose an item: ");
                    int itemChoice;
                    if (scanf("%d", &itemChoice) != 1) { while (getchar()!='\n'); break; }
                    if (itemChoice >= 1 && itemChoice <= player->itemCount) {
                        struct Item item = player->items[itemChoice - 1];
                        if (strcmp(item.effect, "HP+5") == 0) active->HP += 5;
                        else if (strcmp(item.effect, "HP+10") == 0) active->HP += 10;
                        else if (strcmp(item.effect, "Level+1") == 0) active->level += 1;
                        if (active->HP > active->maxHP) active->HP = active->maxHP;
                        printf("%s used!\n", item.name);
                        itemUsed++;
                    }
                    break;
                }
                case 4: {
                    float fleeChance = (float)active->speed / (active->speed + enemy.speed);
                    if ((rand()%100) < (int)(fleeChance*100)) {
                        printf("You escaped successfully!\n");
                        return;
                    } else {
                        printf("Escape failed!\n");
                    }
                    break;
                }
                case 5: {
                    float captureChance = ((float)(enemy.maxHP - enemy.HP) / enemy.maxHP);
                    if ((rand()%100) < (int)(captureChance*100)) {
                        if (player->supemonCount < 20) {
                            player->supemons[player->supemonCount++] = enemy;
                            printf("%s captured!\n", enemy.name);
                        } else {
                            printf("Your party is full.\n");
                        }
                        return;
                    } else {
                        printf("Capture failed!\n");
                    }
                    break;
                }
                default:
                    printf("Wrong choice\n");
            }
        } else {
            /* tour ennemi */
            printf("\nTurn of %s\n", enemy.name);
            int move = rand() % 2;
            int damage = 0;
            if (strcmp(enemy.moves[move], "Scratch") == 0) damage = 3;
            else if (strcmp(enemy.moves[move], "Pound") == 0) damage = 2;
            else if (strcmp(enemy.moves[move], "Growl") == 0) { enemy.attack += 1; printf("%s increases its attack!\n", enemy.name); playerTurn = 1 - playerTurn; continue; }
            else if (strcmp(enemy.moves[move], "Foliage") == 0) { enemy.evasion += 1; printf("%s increases its evasion!\n", enemy.name); playerTurn = 1 - playerTurn; continue; }
            else if (strcmp(enemy.moves[move], "Shell") == 0) { enemy.defense += 1; printf("%s increases its defense!\n", enemy.name); playerTurn = 1 - playerTurn; continue; }

            float hitChance = (float)enemy.accuracy / (enemy.accuracy + active->evasion) + 0.1f;
            if ((rand()%100) < (int)(hitChance*100)) {
                int finalDamage = randomRound((float)(enemy.attack * damage) / (active->defense>0?active->defense:1));
                active->HP -= finalDamage;
                if (active->HP < 0) active->HP = 0;
                printf("%s deals you %d damage! (Your HP: %d/%d)\n", enemy.name, finalDamage, active->HP, active->maxHP);
            } else {
                printf("%s missed its attack!\n", enemy.name);
            }
        }

        playerTurn = 1 - playerTurn; // switch turn
    }

    if (active->HP <= 0) {
        printf("\nYour Supemon %s is KO. You lost the fight.\n", active->name);
    } else {
        printf("\nYou defeated %s!\n", enemy.name);
        int reward = 100 + rand()%401;
        player->supcoins += reward;
        int xpGain = (100 + rand()%401) * enemy.level;
        active->experience += xpGain;
        printf("Reward : %d Supcoins and %d XP\n", reward, xpGain);

        int nextLevelXP = 500 + (active->level - 1) * 1000;
        while (active->experience >= nextLevelXP) {
            active->level += 1;
            printf("%s goes to level %d!\n", active->name, active->level);

            active->maxHP = randomRound(active->maxHP * 1.3f);
            active->HP = active->maxHP;
            active->attack = randomRound(active->attack * 1.3f);
            active->defense = randomRound(active->defense * 1.3f);
            active->evasion = randomRound(active->evasion * 1.3f);
            active->accuracy = randomRound(active->accuracy * 1.3f);
            active->speed = randomRound(active->speed * 1.3f);

            nextLevelXP = 500 + (active->level - 1) * 1000;
        }
    }
}


// Fonction OutOfCombat (menu)

void OutOfCombat(struct Player *player) {
    while (1) {

        printf("\n+---------------------------------------+\n");
        printf("|           OUT OF COMBAT MENU          |\n");
        printf("+---------------------------------------+\n");
        printf("| 1 - Into the Wild                     |\n");
        printf("| 2 - Shop                              |\n");
        printf("| 3 - Supemon Center                    |\n");
        printf("| 4 - Save Game                         |\n");
        printf("| 5 - Leave Game                        |\n");
        printf("+---------------------------------------+\n");
        printf("Choice: ");

        int choice;
        if (scanf("%d", &choice) != 1) { while (getchar()!='\n'); continue; } // verifie si c'est un entier

        switch (choice) {
            case 1: intoTheWild(player); break;
            case 2: shop(player); break;
            case 3: supCenter(player); break;
            case 4: savegame(player); break;
            case 5: printf("Leave the game. Bye\n"); exit(0); break;
            default: printf("Please enter 1-5\n"); break; // verifie si c'est dans choix SWITCH possibles
        }
    }
}


/*  main  */

int main(void) {
    srand((unsigned)time(NULL));

    /* modèles de supemon starter */
    struct Supemon Supmander = { .id = 1, .level = 1, .experience = 0, .HP = 10, .maxHP = 10,
                                .attack = 1, .defense = 1, .evasion = 1, .accuracy = 2, .speed = 1 };
    strcpy(Supmander.name, "Supmander");
    strcpy(Supmander.moves[0], "Scratch");
    strcpy(Supmander.moves[1], "Growl");

    struct Supemon Supasaur = { .id = 2, .level = 1, .experience = 0, .HP = 9, .maxHP = 9,
                               .attack = 1, .defense = 1, .evasion = 3, .accuracy = 2, .speed = 2 };
    strcpy(Supasaur.name, "Supasaur");
    strcpy(Supasaur.moves[0], "Pound");
    strcpy(Supasaur.moves[1], "Foliage");

    struct Supemon Supirtle = { .id = 3, .level = 1, .experience = 0, .HP = 11, .maxHP = 11,
                               .attack = 1, .defense = 2, .evasion = 2, .accuracy = 1, .speed = 2 };
    strcpy(Supirtle.name, "Supirtle");
    strcpy(Supirtle.moves[0], "Pound");
    strcpy(Supirtle.moves[1], "Shell");

    struct Player player;
    player.supemonCount = 0;
    player.itemCount = 0;
    player.supcoins = 200;
    player.selectedSupemonIndex = -1;

    int loadChoice;
    do {
        printf("1- New game\n2- Load save\nChoice: ");
        if (scanf("%d", &loadChoice) != 1) { while (getchar()!='\n'); loadChoice = 0; }
    } while (loadChoice < 1 || loadChoice > 2);

    if (loadChoice == 1) {
        printf("Enter your name: ");
        if (scanf("%19s", player.name) != 1) strncpy(player.name, "Player", sizeof(player.name));
        printf("Hello %s!\n", player.name);

        printf("\n+---------------------------------------+\n");
        printf("|         WELCOME TO SUPEMON WORLD      |\n");
        printf("+---------------------------------------+\n");
        printf("| Choose your starter Supemon :         |\n");
        printf("|                                       |\n");
        printf("|   1 - Supmander                       |\n");
        printf("|   2 - Supasaur                        |\n");
        printf("|   3 - Supirtle                        |\n");
        printf("+---------------------------------------+\n");

        int choice;
        do {
            printf("Your choice (1, 2, or 3): ");
            if (scanf("%d", &choice) != 1) { while (getchar()!='\n'); choice = 0; }
        } while (choice < 1 || choice > 3);

        if (choice == 1) player.supemons[player.supemonCount++] = Supmander;
        else if (choice == 2) player.supemons[player.supemonCount++] = Supasaur;
        else player.supemons[player.supemonCount++] = Supirtle;

        player.selectedSupemonIndex = 0;
    } else {
        /* charge la sauvegarde si existante, sinon nouvelle partie */
        loadgame(&player);
        if (player.supemonCount == 0) {
            /* aucun supemon dans la sauvegarde => proposer starter */
            printf("No supemon in save. Starting new trainer.\n");
            printf("Enter your name: ");
            if (scanf("%19s", player.name) != 1) strncpy(player.name, "Player", sizeof(player.name));
            player.supemons[player.supemonCount++] = Supmander;
            player.selectedSupemonIndex = 0;
        }
    }

    /* Lancer la boucle principale hors-combat */
    OutOfCombat(&player);

    return 0;
}