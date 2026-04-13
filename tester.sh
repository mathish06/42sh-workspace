#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

if [ ! -f "./42sh" ]; then
    echo "❌ Fais un 'make' d'abord."
    exit 1
fi

if [ ! -f "./tests/tests.txt" ]; then
    echo "❌ Fichier tests.txt introuvable."
    exit 1
fi

TEST_COUNT=0
PASSED=0

# On lit le fichier ligne par ligne
while IFS= read -r cmd || [ -n "$cmd" ]; do
    # On ignore les lignes vides
    if [[ -z "$cmd" ]]; then
        continue
    fi
    
    # On affiche les commentaires en bleu (comme des titres)
    if [[ "$cmd" == \#* ]]; then
        echo -e "\n${BLUE}$cmd${NC}"
        continue
    fi

    ((TEST_COUNT++))

    echo "$cmd" | tcsh > ref.out 2> ref.err
    ref_status=$?

    echo "$cmd" | ./42sh > my.out 2> my.err
    my_status=$?

    diff ref.out my.out > diff.out
    diff_out=$?
    
    diff ref.err my.err > diff.err
    diff_err=$?

    if [ $diff_out -eq 0 ] && [ $diff_err -eq 0 ] && [ $ref_status -eq $my_status ]; then
        echo -e "${GREEN}[OK]${NC} $cmd"
        ((PASSED++))
    else
        echo -e "${RED}[KO]${NC} $cmd"
        if [ $ref_status -ne $my_status ]; then
            echo "  -> Code retour: tcsh=$ref_status | 42sh=$my_status"
        fi
        if [ $diff_out -ne 0 ]; then
            echo "  -> Différences Stdout (< tcsh | > 42sh) :"
            cat diff.out | head -n 5 # Affiche juste les 5 premières lignes du diff pour pas polluer
        fi
    fi
done < tests/tests.txt

# Résumé final
echo -e "\n======================="
echo -e "Résultat : $PASSED / $TEST_COUNT tests réussis."
echo -e "======================="

rm -f ref.out ref.err my.out my.err diff.out diff.err test_out.txt out1.txt out2.txt