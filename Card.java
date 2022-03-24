/*
 * Card.java
 *
 * A blueprint class to represent an individual playing card.
 *
 * CS 112, Boston University
 *
 * completed by: Stone Harris, stoneh@bu.edu
 */

public class Card {
    // constants for the ranks of non-numeric cards
    public static final int ACE = 1;
    public static final int JACK = 11;
    public static final int QUEEN = 12;
    public static final int KING = 13;

    // other constants for the ranks
    public static final int FIRST_RANK = 1;
    public static final int LAST_RANK = 13;

    // Arrays of strings for the rank names and abbreviations.
    // The name of the rank r is given by RANK_NAMES[r].
    // The abbreviation of the rank r is given by RANK_ABBREVS[r].
    private static final String[] RANK_NAMES = {
            null, "Ace", "2", "3", "4", "5", "6",
            "7", "8", "9", "10", "Jack", "Queen", "King"
    };
    private static final String[] RANK_ABBREVS = {
            null, "A", "2", "3", "4", "5", "6",
            "7", "8", "9", "10", "J", "Q", "K"
    };

    // constants for the suits
    public static final int FIRST_SUIT = 0;
    public static final int LAST_SUIT = 3;
    public static final int CLUBS = 0;
    public static final int DIAMONDS = 1;
    public static final int HEARTS = 2;
    public static final int SPADES = 3;

    // Arrays of strings for the suit names and abbreviations.
    // The name of the suit s is given by SUIT_NAMES[s].
    // The abbreviation of the suit s is given by SUIT_ABBREVS[s].
    private static final String[] SUIT_NAMES = {
            "Clubs", "Diamonds", "Hearts", "Spades"
    };
    private static final String[] SUIT_ABBREVS = {
            "C", "D", "H", "S"
    };

    /***** part 2: getSuitNum *****/
    //Takes in a suit in the form of a string and returns the index of which the suit can be found in the "SUIT_NAMES" array
    private static int getSuitNum(String suit) {
        // The return statement below is included so the starter code
        // will compile.
        // Replace it with your implementation of the method.

        for (int i = 0; i <SUIT_NAMES.length; i++) {
            if (SUIT_NAMES[i].equalsIgnoreCase(suit)) {
                return i;
            }
        }
        return -1;
    }


    /***** Implement parts 3-7 below. *****/
    //3. Here are the fields being defined
    private int rank;
    private int suitNum;

    //4. constructors: first one takes in int rank and int suitNum and assigns them to the corresponding fields
    public Card(int rank, int suitNum) {
        if (rank > 13 || rank < 0) {
            throw new IllegalArgumentException("Illegal Argument");
        } else if (suitNum > 3 || suitNum < 0) {
            throw new IllegalArgumentException("Illegal Argument");
        }
        this.rank = rank;
        this.suitNum = suitNum;

    }
    // second one takes in int rank and assigns it to corresponding field, takes in string suit, converts it to an int via getSuitNum() then assigns it to correct field
    public Card(int rank, String suit) {
        if (rank > 13 || rank < 0) {
            throw new IllegalArgumentException("Illegal Argument");
        }
        int count = 0;
        for (int i = 0; i < SUIT_NAMES.length; i++) {
            if (SUIT_NAMES[i].equalsIgnoreCase(suit)) {
            } else {
                count++;
            }
        }
        if (count == 4) {
            throw new IllegalArgumentException("Illegal Argument");
        }
        this.rank = rank;
        this.suitNum = getSuitNum(suit);

    }

    //5.
    //returns rank of given card
    public int getRank() {
        return this.rank;
    }

    //returns a string describing the rank name of a given card
    public String getRankName() {
        return RANK_NAMES[rank];
    }

    //returns suit number of a given card
    public int getSuitNum() {
        return this.suitNum;
    }

    //returns the correct suit name for a given card by accessing the element of array SUIT_NAMES that corresponds to the card's suit number
    public String getSuitName() {
        return SUIT_NAMES[getSuitNum()];
    }

    //returns a string that consists of "rank_name of suit_name" for a given card
    public String getName() {
        String res = getRankName() + " of " + getSuitName();
        return res;
    }

    //returns a boolean that clarifies whether a card is an ace or not
    public boolean isAce() {
        if (getRank() == 1) {
            return true;
        } else {
            return false;
        }
    }

    //returns a boolean that signifies whether a card is a face card or not
    public boolean isFaceCard() {
        if (getRank() > 10) {
            return true;
        } else {
            return false;
        }
    }

    //returns the rank of a card, unless it is a face card. in that case it returns 10
    public int getValue() {
        if (isFaceCard()) {
            return 10;
        } else {
            return getRank();
        }

    }

    //6.
    public String toString() {
        return RANK_ABBREVS[getRank()] + SUIT_ABBREVS[getSuitNum()];
    }

    //7.
    //evaluates whether a given card is the same suit, returning a boolean to indicate the answer
    //returns false if the given card is null
    public boolean sameSuitAs(Card card) {
        if (card == null) {
            return false;
        }
        if (getSuitNum() == card.getSuitNum()) {
            return true;
        } else {
            return false;
        }
    }

    //determines whether a given card is the same rank and suit as another card, returning a boolean to indicate the answer
    //returns false if the given card is null
    public boolean equals(Card card) {
        if (card == null) {
            return false;
        }
        if (getRank() == card.getRank() && sameSuitAs(card)) {
            return true;
        } else {
            return false;
        }
    }

}