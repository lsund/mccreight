#include "test.h"

Uchar *text;
Uint textlen, max_codepoint;

static Uint min(const Uint a, const Uint b)
{
    return a < b ? a : b;
}

char *test_count(char *patternfile, char *textfile, Uint count)
{
    Uint patternslen;
    /* setlocale(LC_ALL, "en_US.utf8"); */
    FILE *in = fopen(textfile, "r");
    text = malloc(sizeof(Uchar) * MAXTEXTLEN);
    char c;
    textlen = 0;
    while ((c = fgetc(in)) != EOF) {
        text[textlen] = c;
        textlen++;
    }
    text[textlen + 1] = '\0';
    /* max_codepoint = get_max(wtext, textlen); */
    fclose(in);
    char **patterns = (char **) malloc(sizeof(char *) * MAX_PATTERNS);
    int npatterns  = file2Array(patternfile, &patternslen, MAX_PATTERNS, &patterns);

    Uint exists_n = 0;

    for (Uint j = 0; j < (Uint) npatterns; j++) {

        /* char *current_pattern = patterns[j]; */
        /* Uint patternlen = strlen(current_pattern); */

        /* bool exists = search_pattern(current_pattern, patternlen); */

        /* exists ? exists_n++ : (void) 0; */
    }
    for (int i = npatterns - 1; i >= 0; i--) {
        free(patterns[i]);
    }

    free(patterns);

    mu_assert("Count should be correct", exists_n == count);

    return NULL;
}

char *compare_vs_naive(char *patternfile, char *textfile)
{
    int maxpatterns = 50;

    Uint patternslen;
    /* setlocale(LC_ALL, "en_US.utf8"); */
    FILE *in = fopen(textfile, "r");
    text = malloc(sizeof(Uchar) * MAXTEXTLEN);
    char c;
    textlen = 0;
    while ((c = fgetc(in)) != EOF) {
        text[textlen] = c;
        textlen++;
    }
    text[textlen + 1] = '\0';

    /* max_codepoint = get_max(wtext, textlen); */
    fclose(in);
    Uchar **patterns = (Uchar **) malloc(sizeof(Uchar *) * MAX_PATTERNS);
    int npatterns  = file_to_strings(patternfile, &patternslen, MAX_PATTERNS, &patterns);
    Suffixtree stree;
    constructstree(&stree, text, textlen);

    for (Uint i = 0; i < MAX_CHARS; i++) {
        if (*(stree.rootchildren + i) != UNDEFREFERENCE) {
            printf("%lu\n", *(stree.rootchildren + i));
        }
    }



    /* int exists_n = 0, rexists_n = 0; */
    for (Uint j = 0; j < min(npatterns, maxpatterns); j++) {

        Uchar *current_pattern = patterns[j];

        Uint patternlen = strlen((char *) current_pattern);

        Uchar *end = current_pattern + patternlen;

        /* bool exists = search_pattern(current_pattern, patternlen); */
        bool rexists = naive_search(current_pattern, end);
        printf("%d\n", rexists);

        /* exists ? exists_n++ : (void) 0; */
        /* rexists ? rexists_n++ : (void) 0; */

        /* if (rexists != exists) { */
        /*     printf("%d %d\n", rexists, exists); */
        /*     printf("Fail on: %ls\n", patterns[j]); */
        /* } */
        /* mu_assert( */
        /*     "Naive and suffix tree sourch should be the same.", */
        /*     rexists == exists */
        /* ); */
    }
    for (int i = npatterns - 1; i >= 0; i--) {
        free(patterns[i]);
    }
    free(patterns);
    return NULL;
}

char *utest_search()
{

    char *error;

    mu_message(DATA, "Trivial\n");
    error = compare_vs_naive(
                "data/trivial-patterns.txt",
                "data/trivial.txt"
            );
    if (error) return error;

    /* mu_message(DATA, "Random existing patterns\n"); */
    /* error = compare_vs_naive( */
    /*             "data/members/random-patterns.txt", */
    /*             "data/members/diffsize/005.txt" */
    /*         ); */
    /* if (error) return error; */

    /* mu_message(DATA, "Random non-existing patterns\n"); */
    /* error = compare_vs_naive( */
    /*             "data/members/random-patterns-non-existing.txt", */
    /*             "data/members/diffsize/005.txt" */
    /*         ); */
    /* if (error) return error; */

    /* mu_message(DATA, "Akz patterns\n"); */
    /* error = compare_vs_naive( */
    /*             "data/akz/10000.txt", */
    /*             "data/akz/data.xml" */
    /*         ); */
    /* if (error) return error; */

    /* mu_message(DATA, "Akz patterns, take 2\n"); */
    /* error = compare_vs_naive( */
    /*             "data/doctronic/diffsize/12000.txt", */
    /*             "data/doctronic/data-diffsize/small.xml"); */
    /* if (error) return error; */

    /* mu_message(DATA, "Count: Akz patterns\n"); */
    /* error = test_count( */
    /*             "data/akz/10000.txt", */
    /*             "data/akz/data.xml", */
    /*             174 */
    /*         ); */
    /* if (error) return error; */

    /* mu_message(DATA, "Count: Akz patterns, take 2\n"); */
    /* error = test_count( */
    /*             "data/doctronic/diffsize/12000.txt", */
    /*             "data/doctronic/data-diffsize/small.xml", */
    /*             170 */
    /*         ); */
    /* if (error) return error; */

    return NULL;
}

char *test_search()
{
    mu_run_utest(utest_search);

    return NULL;
}

