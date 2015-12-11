#!/usr/bin/python
# analyse_keys_logs.py

import argparse
import glob
import re
import string


# <p class="keystrokes" time="19:16:13" name="cmd" >19:16:13 vagrant up^M</p>
PATTERN = "<p class=\"keystrokes\" time=\".*\" name=\".*\" >\d+:\d+:\d+ (.*)</p>"
PATTERN_MULTI_FIRST = "<p class=\"keystrokes\" time=\".*\" name=\".*\" >\d+:\d+:\d+ (.*)"
PATTERN_MULTI_LAST = "(.*)</p>"

ACCENTUATED_CODES = [224, 226, 230, 231, 232, 233, 234, 238, 239, 244, 249, 251, 339]
# a` 224
# a^ 226
# ae 230
# c, 231
# e` 232
# e' 233
# e^ 234
# i^ 238
# i: 239
# o^ 244
# u` 249
# u^ 251
# oe 339

OTHER_SPECIAL_CODES = [176]


parser = argparse.ArgumentParser()
parser.add_argument("-d", "--debug", help="display debug information", action="store_true")
args = parser.parse_args()

compiled_pattern = re.compile(PATTERN)
compiled_pattern_multi_first = re.compile(PATTERN_MULTI_FIRST)
compiled_pattern_multi_last = re.compile(PATTERN_MULTI_LAST)

letter_count = {}
punctuation_count = {}
digram_count = {}

files_names = glob.glob('*.htm')
for file_name in files_names:
    print "analysing '%s'" % file_name

    with open(file_name) as file:
        multi_line = None

        for line in file:

            # handle keystrokes in one line, or in multi-line
            keystrokes = None
            if multi_line:
                match = compiled_pattern_multi_last.match(line)
                if not match:
                    multi_line += line
                    continue
                keystrokes = multi_line + " " + match.group(1)
                multi_line = None
            else:
                match = compiled_pattern.match(line)
                if not match:
                    match = compiled_pattern_multi_first.match(line)
                    if match:
                        multi_line = match.group(1)
                    continue
                if match:
                    keystrokes = match.group(1)

            # here keystrokes should not be None
            keystrokes = keystrokes.decode('utf-8').lower()
            if args.debug:
                print "\nkeystrokes: '%s'" % keystrokes

            previous_letter = None
            length = len(keystrokes)
            i = 0
            while (i < length):
                character = keystrokes[i]
                code_ascii = ord(character)
                punctuation = False
                letter = False
                if (code_ascii >= 33) and (code_ascii <= 96):
                    punctuation = True
                elif (code_ascii >= 97) and (code_ascii <= 122):
                    letter = True
                elif (code_ascii >= 123) and (code_ascii <= 126):
                    punctuation = True
                elif code_ascii in ACCENTUATED_CODES:
                    letter = True
                elif code_ascii in OTHER_SPECIAL_CODES:
                    punctuation = True
                if letter:
                    if args.debug:
                        print "letter: %s" % character
                    if character in letter_count.keys():
                        letter_count[character] += 1
                    else:
                        letter_count[character] = 1
                    if previous_letter:
                        digram = previous_letter + character
                        if args.debug:
                            print "digram: %s" % digram
                        if digram in digram_count.keys():
                            digram_count[digram] += 1
                        else:
                            digram_count[digram] = 1
                    previous_letter = character
                elif punctuation:
                    if args.debug:
                        print "punctuation: %s" % character
                    if character in punctuation_count.keys():
                        punctuation_count[character] += 1
                    else:
                        punctuation_count[character] = 1
                    previous_letter = None
                else:
                    previous_letter = None

                i += 1

letter_count_sum = sum(letter_count.values())
print "\nletters frequencies (number of collected letters: %s):" % letter_count_sum
for letter in sorted(letter_count.keys()):
    print "   %s: %s" % (letter, int(round(float(letter_count[letter]) * 1000 / letter_count_sum)))

punctuation_count_sum = sum(punctuation_count.values())
print "\npunctuations frequencies (number of collected punctuations: %s):" % punctuation_count_sum
for punctuation, count in sorted(punctuation_count.iteritems(), key=lambda(k, v): (v, k), reverse=True):
    print "   %s: %s" % (punctuation, int(round(float(count) * 1000 / punctuation_count_sum)))

if args.debug:
    print "\nall digrams frequencies:"
    digram_count_sum = sum(digram_count.values())
    for digram in digram_count.keys():
        print "   %s: %s" % (digram, int(round(float(digram_count[digram]) * 1000 / digram_count_sum)))

# take the first 30 most frequent digrams
most_frequent_digram_count = {}
for digram, count in sorted(digram_count.iteritems(), key=lambda(k, v): (v, k), reverse=True)[:30]:
    most_frequent_digram_count[digram] = count
most_frequent_digram_count_sum = sum(most_frequent_digram_count.values())
print "\nmost frequent digrams frequencies (number of collected most frequent digrams: %s):" % most_frequent_digram_count_sum
for digram, count in sorted(most_frequent_digram_count.iteritems(), key=lambda(k, v): (v, k), reverse=True):
    print "   %s: %s" % (digram, int(round(float(count) * 1000 / most_frequent_digram_count_sum)))
