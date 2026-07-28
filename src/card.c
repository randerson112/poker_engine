#include "card.h"

#include <string.h>
#include <ctype.h>

const char* value_to_string(Value v) {
    switch (v) {
        case TWO: return "2";
        case THREE: return "3";
        case FOUR: return "4";
        case FIVE: return "5";
        case SIX: return "6";
        case SEVEN: return "7";
        case EIGHT: return "8";
        case NINE: return "9";
        case TEN: return "T";
        case JACK: return "J";
        case QUEEN: return "Q";
        case KING: return "K";
        case ACE: return "A";
        default: return "?";
    }
}

const char* suit_to_string(Suit s) {
    switch (s) {
        case SPADES: return "♠️";
        case DIAMONDS: return "♦️";
        case CLUBS: return "♣️";
        case HEARTS: return "♥️";
        default: return "?";
    }
}

int parse_card(const char* str, Card* out_card) {
    if (!str || strlen(str) != 2) return 0;

    char vc = (char)toupper(str[0]);
    char sc = (char)toupper(str[1]);

    Value v;
    switch (vc) {
        case '2': v = TWO; break;
        case '3': v = THREE; break;
        case '4': v = FOUR; break;
        case '5': v = FIVE; break;
        case '6': v = SIX; break;
        case '7': v = SEVEN; break;
        case '8': v = EIGHT; break;
        case '9': v = NINE; break;
        case 'T': v = TEN; break;
        case 'J': v = JACK; break;
        case 'Q': v = QUEEN; break;
        case 'K': v = KING; break;
        case 'A': v = ACE; break;
        default: return 0;
    }

    Suit s;
    switch (sc) {
        case 'S': s = SPADES; break;
        case 'D': s = DIAMONDS; break;
        case 'C': s = CLUBS; break;
        case 'H': s = HEARTS; break;
    }

    out_card->value = v;
    out_card->suit = s;
    return 1;
}