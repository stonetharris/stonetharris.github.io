NO_OF_CHARS = 256

class FindNonRepeatingCharacters(object):
    """Finds non repeating characters in a string"""

    def __init__(self, phrase):
        self.phrase = phrase


    def getCharCountArray(self, phrase):
        count = [0] * NO_OF_CHARS
        for i in phrase:
            count[ord(i)] += 1
        return count


    def firstNonRepeating(self, phrase):
        count = self.getCharCountArray(phrase)
        index = -1
        k = 0

        for i in phrase:
            if count[ord(i)] == 1:
                index = k
                break
            k += 1

        return index


    def RestOfString(self, phrase):
        count = self.getCharCountArray(phrase)
        index = -1
        k = 0

        for i in phrase:
            if count[ord(i)] == 1:
                index = k
                break
            k += 1

        return phrase[index:]


    def FindFirst(self):
        x = self.firstNonRepeating(self.phrase)
        return self.phrase[x]


    def FindLast(self):
        x = self.firstNonRepeating(self.phrase[::-1])
        return self.phrase[::-1][x]


    def FindAll(self):
        index = -1
        char_list = []
        phrase = self.phrase
        while (index <> len(self.phrase)):
            x = self.firstNonRepeating(phrase)
            char_list.append(phrase[x])
            index += 1
            phrase = self.phrase[index:]
        return char_list


def main():
    word = FindNonRepeatingCharacters('geeksforgeeks')
    # print word.FindFirst()
    # print word.FindLast()
    print word.FindAll()

if __name__ == '__main__':
    main()
