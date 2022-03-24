// by Stone Harris, stoneh@bu.edu
import java.util.*;
public class Player {
    //1.
    private String name;
    public Card[] hand;
    public int numCards;

    //2.
    //constructor, takes in string name value, sets it as name field, also initializes a new hand array
    //also sets numCards to 0 because the player does not have nay cards yet
    public Player(String name) {
        this.name = name;
        this.hand = new Card[Blackjack.MAX_CARDS_PER_PLAYER];
        this.numCards = 0;
    }

    //3. accessor, returns the name of the player
    public String getName() {
        return this.name;
    }

    //4. accessor, returns the number of cards in the player's hand
    public int getNumCards() {
        return this.numCards;
    }

    //5. adds a given card c to the player's hand
    public void addCard(Card c) {
        if (c == null || getNumCards() >= Blackjack.MAX_CARDS_PER_PLAYER) {
            throw new IllegalArgumentException("Card is null");
        }
        this.numCards ++;
        for (int i = 0; i < Blackjack.MAX_CARDS_PER_PLAYER; i++) {
            if (hand[i] == null) {
                hand[i] = c;
                break;
            }
        }
    }

    //6. accessor, takes in an integer index and returns the card that is located at that index in the player's hand
    public Card getCard(int index) {
        if (index > this.hand.length) {
            throw new IllegalArgumentException("index is invalid");
        }
        return(this.hand[index]);
    }

    //7. calculates the total value of a player's hand using the Card class's getValue() method
    // returns the total value
    public int getHandValue() {
        int result = 0;
        for (int i = 0; i < getNumCards(); i++) {
            result += this.hand[i].getValue();
        }
        return result;
    }

    //8. prints the cards in a given hand in the correct format
    public void printHand() {
        for (int i = 0; i < this.getNumCards(); i++) {
            System.out.print(this.hand[i] + "  ");
        }
        System.out.print("(value = ");
        System.out.print(this.getHandValue());
        System.out.print(")");

    }

    //9. returns a boolean of true if the player has blackjack (value of 21 with 2 cards), false if otherwise
    public boolean hasBlackjack() {
        if (this.getHandValue() == 21 && this.getNumCards() == 2) {
            return true;
        } else {
            return false;
        }
    }

    //10. utilizes scanner to see whether the player wants a hit, if yes, returns true, if not returns false
    public boolean wantsHit(Scanner console, Player player) {
        String choice;
        System.out.println("Want another hit, " + player.getName() + " (y/n)?");
        choice = console.nextLine();
        if (choice.equalsIgnoreCase("y")) {
            return true;
        } else {
            return false;
        }
    }

    //11. discards all cards in a hand by setting them all to null
    public void discardCards() {
        for (int i = 0; i < getNumCards(); i++) {
            this.hand[i] = null;
        }
        this.numCards = 0;
    }

}
