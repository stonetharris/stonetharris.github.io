// by Stone Harris, stoneh@bu.edu

import java.util.*;
public class Dealer extends Player{

    //1. field boolean indicating whether the first card is revealed or not with encapsulation
    private boolean firstReveal;

    public boolean isFirstReveal() {
        return firstReveal;
    }

    public void setFirstReveal(boolean firstReveal) {
        this.firstReveal = firstReveal;
    }

    //2. constructor, takes in no parameters and sets the name of the player to 'dealer'
    // then utilizes the superclasses constructor for the rest
    public Dealer() {
        super("dealer");
        this.firstReveal = false;
    }

    //3. sets the boolean that allows the dealer's first card to be revealed
    public void revealFirstCard() {
        this.firstReveal = true;
    }

    //4. prints the dealer's hand correctly, accounting for whether or not their first card is supposed to be shown
    @Override
    public void printHand() {
        if (!this.isFirstReveal()){
            for (int i = 0; i < this.getNumCards(); i++) {
                if (i == 0) {
                    System.out.println("XX");
                } else {
                    System.out.print(getCard(i) + "  ");
                }
            }
        } else {
            for (int i = 0; i < this.getNumCards(); i++) {
                System.out.print(getCard(i) + "  ");
            }
            System.out.print("(value = ");
            System.out.print(this.getHandValue());
            System.out.print(")");
        }
    }

    //5. determines whether or not the dealer wants a hit based on the set rules of the game blackjack
    @Override
    public boolean wantsHit(Scanner console, Player player) {
        if (this.hasBlackjack()) {
            return false;
        }
        if (player.getHandValue() < 17 && player.getHandValue() > this.getHandValue()) {
            return true;
        } else if (player.getHandValue() >= 17 && player.getHandValue() >= this.getHandValue()) {
            return true;
        }
        return false;
    }

    //6. discards all cards in a hand by setting them all to null then sets first reveal to false in prep of the next round
    @Override
    public void discardCards() {
        setFirstReveal(false);
        for (int i = 0; i < getNumCards(); i++) {
            this.hand[i] = null;
        }
        this.numCards = 0;
    }

}
