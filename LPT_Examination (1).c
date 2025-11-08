#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
//=================================================================================================================================================================================

char currentUser[100];

void registerUser() {
    char username[50], password[50];
    FILE *fp = fopen("user_pass.txt", "a+");
    if (fp == 0) {
        printf("Error opening file.\n");
        return;
    }

    printf("\nUsername: ");
    scanf("%s", username);

    char fileUser[50], filePass[50];
    while (fscanf(fp, "%s %s", fileUser, filePass) == 2) {
        if (strcmp(username, fileUser) == 0) {
            printf("Username already exists. Try a different one.\n");
            fclose(fp);
            return;

        }
    }

    printf("\nPassword: ");
    scanf("%s", password);


    fprintf(fp, "%s %s\n", username, password);
    printf("Registration successful.\n");

    fclose(fp);

}


//=================================================================================================================================================================================


int loginUser() {
    char username[50], password[50];
    char fileUser[50], filePass[50];
    int found = 0;

    FILE *fp = fopen("user_pass.txt", "r");
    if (fp == 0) {
        printf("No users registered yet.\n");
        printf("Do you want to register? 1 for Yes, 0 for No: ");

        int choice;
        scanf("%d", &choice);
        if (choice == 1) {
            registerUser();
        }
        return 0;
    }

    printf("\nUsername: ");
    scanf("%s", username);
    printf("\nPassword: ");
    scanf("%s", password);


    while (fscanf(fp, "%s %s", fileUser, filePass) == 2) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            found = 1;
            strcpy(currentUser, username);
            break;
        }
    }

    fclose(fp);

    if (found) {
        system("cls");
        printf("\nWelcome, Examinee %s\n", username);
        return 1;
    } else {
        printf("Invalid username or password.\n");
        printf("Repeat Login? 1 for Yes, 0 for No: ");
        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            return loginUser();
        } else {
            printf("Please refresh the program.\n");
            return 0;
        }
    }
}


//=================================================================================================================================================================================




//=================================================================================================================================================================================


void saveExamScore(const char *subject, int score, int total, float percent, const char *username) {
    FILE *fp = fopen("exam_scores.txt", "a");
    if (fp == 0) {
        printf("Error opening score file.\n");
        return;
    }

    time_t now = time(0);
    struct tm *t = localtime(&now);

    char formattedDate[50];
    strftime(formattedDate, sizeof(formattedDate), "%m/%d/%Y %a %H:%M", t);

    fprintf(fp, "=========================================\n");
    fprintf(fp, "%-15s : %s\n", "Subject", subject);
    fprintf(fp, "%-15s : %d/%d\n", "Score", score, total);
    fprintf(fp, "%-15s : %.2f%%\n", "Percent Score", percent);
    fprintf(fp, "%-15s : %s\n", "Date Taken", formattedDate);
    fprintf(fp, "%-15s : %s\n", "Taken by", username);
    fprintf(fp, "=========================================\n\n");

    fclose(fp);
}


//=================================================================================================================================================================================


void takeExam(const char *subject, char questions[][500], char options[][4][300], char answers[], int totalQuestions) {
    int score = 0;
    char userAnswer;

    printf("\n--- %s Exam ---\n", subject);

    for (int i = 0; i < totalQuestions; i++) {
        printf("\nQuestion %d: %s\n", i + 1, questions[i]);
        printf("A. %s\n", options[i][0]);
        printf("B. %s\n", options[i][1]);
        printf("C. %s\n", options[i][2]);
        printf("D. %s\n", options[i][3]);

        while (1) {
            printf("\nYour answer (A/B/C/D): ");
            scanf(" %c", &userAnswer);
            userAnswer = toupper(userAnswer);

            if (userAnswer >= 'A' && userAnswer <= 'D') {
                break;
            } else {
                printf("Invalid input. Please enter only A, B, C, or D.\n");
            }
        }

        if (userAnswer == answers[i]) {
            score++;
        }
    }

    float percent = ((float)score / totalQuestions) * 100.0;

    printf("\n=== %s Results ===\n", subject);
    printf("You got %d out of %d correct in %s.\n", score, totalQuestions, subject);
    printf("Your score: %.2f%%\n", percent);
    printf("\n");

    saveExamScore(subject, score, totalQuestions, percent, currentUser);
    printf("Your score has been saved successfully.\n");

    printf("Do you want to retake the exam? (1 for Yes, 0 for No): ");
    int choice;
    scanf("%d", &choice);
    if (choice == 1) {
        takeExam(subject, questions, options, answers, totalQuestions);
    } else {
        printf("\nThank you.\n");
    }
}


//=================================================================================================================================================================================


void showLatestExam(const char *username) {
    FILE *fp = fopen("exam_scores.txt", "r");
    if (fp == 0) {
        printf("1. General Education\n");
        printf("2. Professional Education\n");
        printf("3. Specialized Subject\n");
        return;
    }

    char line[500];
    char currentRecord[1000] = "";
    char latestGE[1000] = "";
    char latestPE[1000] = "";
    char latestSS[1000] = "";

    int inRecord = 0;

    while (fgets(line, sizeof(line), fp)) {
    if (strstr(line, "=========================================")) {
    if (inRecord) {
    if (strstr(currentRecord, username)) {
    if (strstr(currentRecord, "General Education"))
            strcpy(latestGE, currentRecord);
    else if (strstr(currentRecord, "Professional Education"))
            strcpy(latestPE, currentRecord);
    else if (strstr(currentRecord, "Specialized Subject"))
            strcpy(latestSS, currentRecord);
}
    currentRecord[0] = '\0';
        inRecord = 0;
    } else {
        strcpy(currentRecord, line);
        inRecord = 1;
}
    } else if (inRecord) {
        strcat(currentRecord, line);
    }
}
fclose(fp);

    printf("\n=== Latest Exams for Examinee: %s ===\n", username);

    if (strlen(latestGE) > 0)
        printf("\n1. General Education\n %s \n", latestGE);
    else
        printf("1. General Education.\n");

    if (strlen(latestPE) > 0)
        printf("\n2. Professional Education\n %s \n", latestPE);
    else
        printf("2. Professional Education.\n");

    if (strlen(latestSS) > 0)
        printf("\n3. Specialized Subject\n %s \n", latestSS);
    else
        printf("3. Specialized Subject.\n");
}


//=================================================================================================================================================================================


int main() {
    int choice;
    int loggedIn = 0;

    while (!loggedIn) {
        printf("\n--- LPT Examination ---\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loggedIn = loginUser();
                break;
            case 3:
                printf("Thank you for using the LPT Examination System.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

//=================================================================================================================================================================================

char GeneralEducationQuestions[100][500] = {
    "Which sentence is written in the active voice?",
    "Choose the correct plural form of analysis.",
    "What figure of speech is used in 'The wind whispered through the trees'?",
    "Which of the following is a complex sentence?",
    "The synonym of 'benevolent' is:",
    "What is the antonym of 'conceal'?",
    "The main idea of a paragraph is usually found in the:",
    "'He is a walking dictionary.' is an example of:",
    "The past participle of 'go' is:",
    "Choose the correct sentence.",
    "What is the correct order of adjectives?",
    "Which word is closest in meaning to 'arduous'?",
    "Choose the correct spelling.",
    "The function of a conjunction is to:",
    "Which word is a preposition?",
    "Choose the correct reported speech: 'I am happy,' she said.",
    "'To kill two birds with one stone' means:",
    "The word 'childish' has a negative connotation compared to 'youthful'.",
    "Identify the underlined word's function: 'She bought a new bag.'",
    "Choose the correct comparative form.",
    "Ang salitang 'maganda' ay isang:",
    "Ano ang kasingkahulugan ng 'maligaya'?",
    "Ang kabaligtaran ng 'mabilis' ay:",
    "Ang pangunahing tauhan sa isang kwento ay tinatawag na:",
    "Ang 'Bahala na' ay halimbawa ng:",
    "Ang pandiwa ay nagpapahayag ng:",
    "'Ang pag-ibig ay parang ulan.' ay halimbawa ng:",
    "'Nagtanim siya ng hangin, bagyo ang aanihin.' ay isang:",
    "Ang 'Balagtasan' ay:",
    "Ang 'Florante at Laura' ay isinulat ni:",
    "Ang 'Alamat' ay isang uri ng:",
    "Sino ang Ama ng Wikang Pambansa?",
    "Ang 'Noli Me Tangere' ay isinulat upang:",
    "Ang tula ay karaniwang may:",
    "Ang 'Panghalip' ay:",
    "Ang kasingkahulugan ng 'matipid' ay:",
    "Ang kabaligtaran ng 'mapagbigay' ay:",
    "Ang panaguri ay tumutukoy sa:",
    "Ang 'Bugtong' ay:",
    "Ang 'Kundiman' ay isang uri ng:",
    "What is the value of 7 × 8 + 4 ÷ 2?",
    "Solve for x: 5x – 10 = 15",
    "What is 20% of 150?",
    "The greatest common factor (GCF) of 24 and 36 is:",
    "The simplest form of the ratio 15:45 is:",
    "Find the median of 3, 5, 7, 9, 11.",
    "If the perimeter of a square is 20 cm, what is the length of one side?",
    "Convert 3/4 to percent.",
    "The sum of angles in a triangle is:",
    "Find the area of a rectangle with length = 8 m and width = 6 m.",
    "What is the next term in the sequence 2, 4, 8, 16, ___?",
    "Simplify: 6^2 – 4^3 ÷ 2",
    "Which of the following numbers is prime?",
    "What is 3/5 of 50?",
    "If 5x = 45, find x.",
    "A bag contains 3 red, 4 blue, and 5 green balls. What is the probability of picking a red ball?",
    "Convert 2.5 hours into minutes.",
    "If a car travels 120 km in 3 hours, what is its average speed?",
    "The square root of 81 is:",
    "Which of the following is an irrational number?",
    "The basic unit of life is the:",
    "Which planet is known as the Red Planet?",
    "Water boils at what temperature at sea level (°C)?",
    "The process by which plants make food is called:",
    "The gas essential for human breathing is:",
    "The force that pulls objects toward the Earth is:",
    "The Earth revolves around the Sun in approximately:",
    "Which organ pumps blood throughout the body?",
    "The largest planet in the solar system is:",
    "Which organelle is known as the powerhouse of the cell?",
    "Which part of the plant absorbs water and nutrients?",
    "The hardest natural substance on Earth is:",
    "The process of water changing from liquid to gas is:",
    "What type of energy is stored in food?",
    "Which blood type is known as the universal donor?",
    "The Sun is a:",
    "What is the main source of energy on Earth?",
    "What organ helps humans breathe?",
    "Which of these is a renewable energy source?",
    "The study of earthquakes is called:",
    "Who is the first President of the Philippines?",
    "The Philippine national hero is:",
    "The national language of the Philippines is:",
    "Philippine Independence Day is celebrated every:",
    "The longest river in the Philippines is:",
    "The Battle of Mactan took place in:",
    "Who wrote 'Mi Ultimo Adios'?",
    "The Philippine national bird is the:",
    "The smallest province in the Philippines is:",
    "The currency of the Philippines is:",
    "Which continent is the Philippines part of?",
    "The highest mountain in the Philippines is:",
    "The executive branch of the government is headed by the:",
    "The Philippine flag has how many rays of the sun?",
    "The three main island groups of the Philippines are:",
    "The national hero known as 'The Great Plebian' is:",
    "The national flower of the Philippines is:",
    "The branch of government that makes laws is:",
    "The Supreme Court belongs to which branch?",
    "The national anthem of the Philippines is:"
};

char GeneralEducationOptions[100][4][300] = {
    {"The book was read by Maria", "The project was completed by the team", "Maria read the book", "The lesson was discussed by the teacher"},
    {"Analysises", "Analysiseses", "Analysi", "Analyses"},
    {"Simile", "Personification", "Metaphor", "Hyperbole"},
    {"She came, she saw, she conquered", "I ran fast and won the race", "Because it rained, we stayed inside", "I like apples and oranges"},
    {"Cruel", "Kind", "Harsh", "Proud"},
    {"Hide", "Reveal", "Cover", "Mask"},
    {"First sentence", "Middle sentence", "Last sentence", "Any part of the paragraph"},
    {"Simile", "Hyperbole", "Metaphor", "Irony"},
    {"Go", "Goes", "Went", "Gone"},
    {"She don’t like ice cream", "She doesn’t likes ice cream", "She doesn’t like ice cream", "She don’t likes ice cream"},
    {"Size, opinion, color, noun", "Opinion, size, color, noun", "Color, size, opinion, noun", "Opinion, color, size, noun"},
    {"Easy", "Difficult", "Light", "Pleasant"},
    {"Occurence", "Occurrence", "Ocurence", "Ocurrance"},
    {"Modify nouns", "Join words or clauses", "Express strong feelings", "Replace nouns"},
    {"Jump", "Beside", "Quickly", "Large"},
    {"She said she was happy", "She says she is happy", "She said she is happy", "She says she was happy"},
    {"To harm animals", "To do two things at once", "To be cruel", "To miss two opportunities"},
    {"True", "False", "Both", "None"},
    {"Verb", "Noun", "Adjective", "Adverb"},
    {"More better", "Gooder", "Better", "Best"},
    {"Pangngalan", "Pang-uri", "Pang-abay", "Panghalip"},
    {"Malungkot", "Masaya", "Tahimik", "Malamig"},
    {"Mabagal", "Matulin", "Mabilis din", "Magaling"},
    {"Bidang kontrabida", "Protagonista", "Antagonista", "Tagapagsalaysay"},
    {"Pananampalataya", "Bahala system", "Bahala expression", "Pagpapaliban"},
    {"Katangian", "Pangalan", "Aksyon", "Damdamin"},
    {"Tula", "Pagtutulad (simile)", "Metapora", "Personipikasyon"},
    {"Salawikain", "Bugtong", "Kasabihan", "Sawikain"},
    {"Debate na patula", "Maikling kwento", "Talumpati", "Dula"},
    {"Jose Rizal", "Francisco Balagtas", "Andres Bonifacio", "Lope K. Santos"},
    {"Kwento", "Tula", "Dula", "Sanaysay"},
    {"Jose Rizal", "Manuel L. Quezon", "Lope K. Santos", "Andres Bonifacio"},
    {"Upang magbigay ng libangan", "Upang magturo ng tula", "Upang ilahad at kritikahin ang katiwalian ng kolonyal na pamahalaan", "Upang magturo ng agham"},
    {"Malaking talata", "Sukat at tugma", "Pawikas", "Prosa"},
    {"Pangngalan", "Pang-uri", "Panghalip", "Pang-abay"},
    {"Gastos", "Tipid", "Maluwag", "Mapang-abuso"},
    {"Mapag-impok", "Mapagmalaki", "Mapagbigay", "Masinop"},
    {"Paksa", "Bahagi ng pangungusap na nagsasaad ng tungkol sa simuno (predicate)", "Uri ng salita", "Uri ng pandiwa"},
    {"Bugtong (bugtong/riddle)", "Salawikain", "Kasabihan", "Sawikain"},
    {"Awit/panaginip", "Awit (tradisyunal na awit pagmamahalan)", "Dula", "Tulang pampanitikan"},
    {"60", "58", "59", "56"},
    {"3", "4", "5", "6"},
    {"25", "30", "35", "40"},
    {"6", "8", "12", "18"},
    {"1:3", "3:1", "5:15", "3:5"},
    {"5", "7", "9", "8"},
    {"4 cm", "5 cm", "6 cm", "8 cm"},
    {"50%", "60%", "75%", "80%"},
    {"90°", "120°", "180°", "360°"},
    {"14 m^2", "48 m^2", "28 m^2", "30 m^2"},
    {"18", "20", "24", "32"},
    {"20", "16", "14", "18"},
    {"12", "15", "19", "21"},
    {"25", "30", "35", "40"},
    {"8", "9", "10", "12"},
    {"1/4", "1/5", "1/3", "2/7"},
    {"100", "120", "150", "180"},
    {"30 km/h", "40 km/h", "50 km/h", "60 km/h"},
    {"8", "9", "10", "11"},
    {"1.5", "√2", "0.25", "5"},
    {"Atom", "Cell", "Tissue", "Organ"},
    {"Mercury", "Venus", "Mars", "Jupiter"},
    {"50°C", "75°C", "100°C", "150°C"},
    {"Respiration", "Transpiration", "Photosynthesis", "Digestion"},
    {"Nitrogen", "Carbon dioxide", "Oxygen", "Hydrogen"},
    {"Friction", "Magnetism", "Gravity", "Inertia"},
    {"24 hours", "7 days", "30 days", "365 days"},
    {"Brain", "Lungs", "Heart", "Kidney"},
    {"Saturn", "Uranus", "Jupiter", "Neptune"},
    {"Nucleus", "Mitochondria", "Ribosome", "Chloroplast"},
    {"Stem", "Leaf", "Root", "Flower"},
    {"Gold", "Iron", "Diamond", "Quartz"},
    {"Condensation", "Evaporation", "Precipitation", "Freezing"},
    {"Kinetic", "Potential", "Chemical", "Thermal"},
    {"A", "B", "AB", "O"},
    {"Planet", "Star", "Comet", "Moon"},
    {"The Sun", "The Moon", "The Ocean", "The Wind"},
    {"Brain", "Lungs", "Heart", "Liver"},
    {"Coal", "Oil", "Wind", "Gas"},
    {"Meteorology", "Seismology", "Geology", "Volcanology"},
    {"Manuel Quezon", "Emilio Aguinaldo", "Jose Rizal", "Andres Bonifacio"},
    {"Emilio Aguinaldo", "Andres Bonifacio", "Jose Rizal", "Apolinario Mabini"},
    {"English", "Tagalog", "Filipino", "Bisaya"},
    {"June 12", "July 4", "May 1", "August 21"},
    {"Pasig River", "Agusan River", "Cagayan River", "Pampanga River"},
    {"1520", "1521", "1565", "1898"},
    {"Andres Bonifacio", "Marcelo H. del Pilar", "Jose Rizal", "Emilio Jacinto"},
    {"Maya", "Eagle", "Haribon (Philippine Eagle)", "Kingfisher"},
    {"Batanes", "Siquijor", "Marinduque", "Guimaras"},
    {"Dollar", "Peso", "Yen", "Baht"},
    {"Africa", "Asia", "Europe", "South America"},
    {"Mt. Mayon", "Mt. Pulag", "Mt. Apo", "Mt. Banahaw"},
    {"Chief Justice", "Senate President", "President", "Speaker of the House"},
    {"6", "8", "12", "16"},
    {"Luzon, Palawan, Sulu", "Luzon, Visayas, Mindanao", "Visayas, Mindanao, Sulu", "Mindoro, Visayas, Palawan"},
    {"Jose Rizal", "Andres Bonifacio", "Emilio Aguinaldo", "Apolinario Mabini"},
    {"Waling-waling", "Sampaguita", "Gumamela", "Ilang-ilang"},
    {"Executive", "Judicial", "Legislative", "Electoral"},
    {"Executive", "Judicial", "Legislative", "Federal"},
    {"Bayan Ko", "Lupang Hinirang", "Ang Bayan Kong Pilipinas", "Pilipinas Kong Mahal"}
};

char GeneralEducationAnswers[100] = {
    'C','D','B','C','B',
    'B','D','C','D','C',
    'B','B','B','B','B',
    'A','B','A','A','C',
    'B','B','A','B','A',
    'C','B','A','A','B',
    'A','B','C','B','C',
    'B','A','B','A','B',
    'C','D','B','C','A',
    'B','B','C','C','B',
    'D','B','C','B','B',
    'A','C','D','B','B',
    'B','C','C','C','C',
    'C','D','C','C','B',
    'C','C','B','C','D',
    'B','A','B','C','B',
    'B','C','C','A','C',
    'B','C','C','A','B',
    'B','C','C','B','B',
    'B','B','C','B','B'
};


//=================================================================================================================================================================================

    char ProfessionalEducationQuestions[100][500] = {
        "Under which category will a globe as an instructional material fall?",
        "Which developmental stage is sometimes called the pre-school years?",
        "Which is (are) NOT supportive of the development of creative thinking?",
        "The following are features of the Re-structures Basic Education Curriculum EXCEPT?",
        "Below is a list of methods used to establish the reliability of the instrument. Which method is questioned for its reliability due to practice and familiarity?",
        "Which one holds true to anecdotal records?",
        "As a teacher you are a rationalist. Which among these will be your guiding principle?",
        "With knowledge explosion, which image of the teacher is unacceptable?",
        "These are also known as combination classes organized in barrios/barangays where the required number of pupils of the same grade level have not met the required number of pupils of the same grade level has not met the required number to make up a separate class thus the teacher apportions class time for instruction to every grade level within class. These are?",
        "Roberto who is in Grade IV has the following grade norm equivalents: MATH-4; Reading-8; and Science-5. What can be said of Roberto? Compared to the average Grade IV pupil, Roberto is",
        "Rights and duties are correlative. This means that",
        "Which of the following activities is the LEAST brain-compatible?",
        "Which physical arrangement of chairs contributes to effective classroom management?",
        "For mastery of learning and in line with the Outcome-Based Evaluation model which element should be present?",
        "As a naturalist, Jean Jacques Rousseau claimed that everyone is essentially good. This view can help the teacher best when?",
        "With specific details in mind, which one (s) has (have) a stronger diagnostic value?",
        "Teacher U teaches to his pupils that pleasure is not the highest good. Teacher's teaching against what philosophy?",
        "Teacher C teaches in a remote multi-grade class. She is seldom visited by her principal and supervisors. She teaches when she feels like teaching and not when she does not feel like teaching. What trait of a good teacher does she lack?",
        "Which may work against collaborative teaching?",
        "Which may work about standard deviation is CORRECT?",
        "With which will the existentialist agree? The school is a place where individuals",
        "A person is held responsible for his actions because",
        "Teacher Q does not want Teacher B to be promoted and so writes an anonymous letters against Teacher B accusing her of fabricated lies. Teacher Q mails this anonymous letter is the School Division Superintendent. What should Teacher Q do if she has to act professionally?",
        "What psychological principle is invoked when a teacher connects the new lesson to the one just completed so that the student may gain a holistic view of the subject?",
        "Which prescribes the abolition of private property by force?",
        "Under which category does the MPS that we talk about in the measurement and evaluation of learning fall?",
        "Which attitude is exemplified by a Boy Scout who says Bahala Na and drives into a pool to save a drowning boy?",
        "My lesson is on methods of family planning. I wanted my class to gain a holistic understanding of family planning methods. So I invited a priest to talk on the morality of family planning methods, a doctor on the physiological aspect of family planning methods, an officer of the Population Commission for his experiences as an officer, and a couple who practiced family planning to talk before my class and to answer questions raised by the class. What technique did I employ?",
        "The State shall protect and promote the right of all citizens to qualify education at all levels. Which government program is in support of this?",
        "Which is the Magna Carta for Public School Teachers?",
        "Which one of the following is NOT a measure of central tendency?",
        "What type of test is given to determine the admission or non-admission of a student to the program?",
        "What is an advantage of the point system of grading?",
        "Which according to Jocano is a strength of the Filipino people?",
        "Which is a sound classroom management practice?",
        "What is the possible effect of an overcrowded curriculum?",
        "Matthew Arnold's ideal person was the person whose powers were all in balance. The following enumerate which has to be developed for a person to become Arnold's ideal person EXCEPT.",
        "How students learn may be more important than what they learn. From this principle, which of the following is particularly important?",
        "If you want a child to climinate an undesirable behavior, punish him. This in accordance to Thorndike's law of",
        "In which cognitive developmental stage is a child unable to distinguish between own perspective and someone else's?",
        "Which is an indicator of a teacher's effectiveness in instilling discipline among children?",
        "Based on Mager's approach in writing lesson objectives, which lesson objective is written correctly?",
        "Which items do not affect the variability of test scores?",
        "The attention to the development of a deep respect and affection for our rich cultural past is an influence",
        "As reported by the Committee on Information Technology, Science, Mathematics Education and other technologies of the 2000 Presidential Commission on Educational Reform, the elementary curriculum is",
        "Which is a feature of exaggerated capitalism?",
        "In the perlance of test construction what does TOS mean?",
        "An industrial Arts teachers is very strict in the classroom, yet he smokes inside the room whenever he wants to. His pupils even see him with his :barkada drinking liquor in public places. If you were the principal, how would you deal with him?",
        "Teacher B wants to diagnose in which vowel sound (s) her students have difficulty. Which tool is most appropriate?",
        "Which one is inimical to nationhood?",
        "The index of difficulty of a particular test is.10. What does this mean? My students",
        "To develop scientific thinking and problem-solving skills, which activity will be most appropriate?",
        "A catalyst teacher is good at the following skills, EXCЕРТ",
        "Which of the 3-id, ego, and superego, consist of instincts?",
        "A good classroom manager has eyes on the back of her head. This means that the effective teacher",
        "Which method is hands-on, minds-on learning?",
        "What is the role of the learner in the Re-structured Basic Education Curriculum?",
        "Which is also called a bar graph?",
        "Which is a good practice that a teacher uses in correcting student's errors?",
        "With knowledge explosion, which image of teacher is unacceptable?",
        "According to Piaget's theory in which developmental stage, can the child do symbolic thinking and go beyond the connection of sensory information and physical action?",
        "The computer is user-friendly. This means that",
        "I am reminded of my Grade I teacher every time I see Miss Valdez because their mannerisms are alike. Which principle of association explains this?",
        "Bullying among kids in school is rampant. What needs to be taught to eradicate it?",
        "Under no circumstances shall a teacher be prejudiced nor discriminatory against any learner, says the Code of Ethics. When is a teacher prejudiced against any learner?",
        "In what developmental stage is growth most rapid?",
        "A woman kills the man who had raped her by reason of self-defense. Is this right?",
        "In which type of grading do teachers evaluate student's learning not in terms of grade but by evaluating the students in terms of expected and mastered skills?",
        "If we teach our students to think creatively, what do we encourage them to do?",
        "Which learning activity is most appropriate if the teacher's focus is attitudinal change?",
        "I want my students to show historical ages graphically which will be most appropriate?",
        "In Piaget's concrete operational stage, what is it that a child can NOT do?",
        "Which is supportive of the development of creative thinking?",
        "With social development in mind, which is most effective?",
        "Which are the characteristics of skill-based instruction for language?",
        "Which question demands the highest level of thinking?",
        "Which is more of a spatial task?",
        "Which principle is observed by Ausabel's schema theory?",
        "Which one uses a projector?",
        "Perrenialism advocates for the development of the cognitive faculties. Which of the following teacher's objective taps higher mental functioning?",
        "Where in the three-level teaching strategy does the teacher pose the question: can we say that the root cause of water pollution is our very own indifference? How can we fight our indifference to the water pollution problem around us?",
        "The teacher prepares a minimum of five questions in her daily lesson plans. In what phase of teaching-learning activities does she take up these questions?",
        "To be rationalist which is the highest faculty of man?",
        "Teacher F teaches the rest of the class while one group works in a study area on a special task. Students share materials help one another, and assess cach ideas and assignments. What approach does the teacher use",
        "The concluding part of the three-level approach is the",
        "The Venn diagram is most fit for lessons on",
        "Man may understand all about the rotation of the earth but he may still miss the radiance of the sunset, said Dr. Martin Luther King. Which is an educational implication of this statement? Stress on",
        "A son put a time bomb in the luggage of his mother who took it abroad from a Philippine airline. The bomb exploded while the airplane was in flight killing the mother and forty other passengers. Although the movie of the criminal act was never revealed by the son, he aroused suspicion, when he named himself beneficiary to an insurance policy he had previously taken out on his mother's life. Was the son accountable for the death of his mother and other passengers?",
        "My lesson is on methods of family planning. I would like my class to have a holistic and a comprehensive understanding of family planning methods. Which technique will be most appropriate?",
        "With the linguistically intelligent group in mind, which activity is least effective?",
        "The mode of a score distribution is 25. this means that",
        "What does a positively skewed score distribution imply?",
        "To have a test with a coverage and with power to test analytical thinking and case scoring? Which type should the teachers use?",
        "Who was a strong supporter of inclusive education and education to all concept?",
        "Which visual display is a result of student's comprehension of a selection read proven by his ability to organize and integrate concepts and information gleaned from the selection?",
        "After showing the chart on families of different sizes and their corresponding water consumption, Teacher L asked pupils this question: What can you say about the size of families and their average water consumption? To answer the question which specific skill is demanded of the pupils?",
        "Each teacher is said to be a trustee of the cultural and educational heritage of the nation and is under obligation to transmit to learners such heritage. Which practice makes him fulfill such an obligation?",
        "Ms. Cho gives a quiz to her class after teaching a lesson. What does she give?",
        "Teacher K believes that every learner can be helped to achieve his full potential and so functions effectively in society when we satisfy his needs. On what theory(ies) is Teacher K's belief based?",
        "Which are the characteristics of a child with ADHD?",

    };
    char ProfessionalEducationOptions[100][4][300] = {
        {"Picture", "Model", "Mock up", "Realia"},
        {"Middle Childhood", "Late Infancy", "Early Childhood", "Early Infancy"},
        {"open classroom atmosphere", "authoritative teacher", "divergent questions", "authoritarian teacher"},
        {"stronger integration of competencies and values within and across the learning areas", "greater emphasis on content, less on the learning process", "increased time for tasks to gain mastery of competencies", "interdisciplinary modes of teaching"},
        {"split half", "equivalent forms", "fest-retest", "Kuder Richardson"},
        {"combining facts with interpretation", "ease in writing objective, descriptive behavior", "describing behavior in natural settings", "describing behavior in a laboratory setting"},
        {"I must teach the child that we can never have real knowledge of anything", "I must teach the child every knowledge, skill, and value that he needs for a better future", "I must teach the child to develop his mental powers to the full", "I must teach the child so he is assured of heaven"},
        {"questionnaire", "guide on the side", "facilitator of learning", "omniscient teacher"},
        {"extension classes", "homogenous classes", "heterogeneous classes", "multi-grade classes"},
        {"behind in Math", "average in Science", "behind in Math and average in Science", "advanced in Science"},
        {"right and duties regulate the relationship of men in society", "cach right carries with it one or several corresponding duties", "rights and duties ultimate come from GOD.", "rights and duties arise from the natural law."},
        {"Explaining how the CPU can function like the brain", "Detecting error in the computer program", "Making a flowchart of a computer process", "Encoding a paragraph with speed and accuracy"},
        {"that which makes it easier to clean the room", "that which distinguishes teacher from students", "that which sticks to the traditional chair arrangement in the classroom", "that which enhances classroom interaction"},
        {"inclusion of non-performance objectives", "construction of criterion-referenced tests", "construction of norm-referenced tests", "non-provision of independent learning"},
        {"dealing with misbehaving students", "helping the slow learners in class", "teaching the students some values", "knowing the students potentials"},
        {"Restricted essay test", "Non-restricted essay test", "Restricted and non-restricted essay test", "Multiple choice test"},
        {"Empiricism", "Epicureanism", "Hedonism", "Realism"},
        {"Emotional intelligence", "Integrity and accountability", "Competence", "Intelligence"},
        {"Establishment of mutual goals", "Joint rewards", "Homogenous grouping", "knee-to-knee seat arrangement by group"},
        {"The lower the standard deviation the more spread the scores are.", "The higher the standard deviation the less spread the scores are.", "The higher the standard deviation the more spread the scores are.", "It is a measure of central tendency."},
        {"can reflect on ideas", "can observe by using their senses to the maximum", "can meet to pursue dialogue and discussion about their own lives and choices", "listen and accept what teachers say"},
        {"he is mature", "he has a choice", "he has instincts", "he has reason"},
        {"Hires a group to distribute poison letters against Teacher B for information dissemination", "Submit a signed justification criticism against teacher B, if there is any", "Instigate student activities to read poison letters over the microphone", "Go straight to the schools Division Superintendent and gives criticism verbally"},
        {"Stimulation", "Conceptualization", "Recognition", "Apperception"},
        {"Moderate capitalism", "Socialism", "Communism", "Exaggerated capitalism"},
        {"Mode", "Mean", "Median", "Ratio"},
        {"A daring attitude", "A segurista attitude", "A daring attitude combined with pagmalasakit", "An I dont-care attitude"},
        {"Forum", "A panel discussion", "Debate", "Symposium"},
        {"Exclusion of children with special needs from the formal system", "Free elementary and secondary education", "Deregulated tuition fee hike", "Re-introduction of the NEAT and NSAT"},
        {"RA 7836", "RA 4670", "RA 7722", "P.D. 101I"},
        {"Median", "Mean", "Variance", "Mode"},
        {"Placement", "Diagnostic", "Aptitude", "Achievement"},
        {"it does away with establishing clear distinctions among students", "It is precise", "It is qualitative", "It emphasizes learning not objectivity of scoring"},
        {"sense of commitment to nationalistic ideals", "sense of historical or national identity", "sense of humor", "sense of national pride"},
        {"Apply rules and policies on a case to case basis", "Apply reactive approach to discipline", "Avoid establishing routines; routines make your students robots", "Establish routines for all daily needs and tasks"},
        {"in-depth learning tends to be given greater emphasis", "There is a greater concept of understanding", "Lifelong learning skills tend to be fully developed", "There is a lack of personal analysis and reflection on major concepts"},
        {"head, heart, hands, health", "knowledge, skills, attitudes", "brain, mind, reason", "cognitive, psychomotor, and affective powers"},
        {"knowing how to solve a problem", "getting the right answer to a world problem", "determining the given", "solving the problem within the time limit"},
        {"Reward", "Exercise", "Punishment", "Effect"},
        {"sensorimotor stage", "concrete-operational stage", "formal-operational stage", "pre-operational stage"},
        {"Teacher's presence is needed for discipline", "Students have developed concern for one another", "Students behave for fear of punishment", "Students behave because of a promised reward"},
        {"At the end of the period, 80 of the class is able to compose a seven-to-ten sentence paragraph that observes unity and clarity", "At the end of the period, 80 of the class is able to solve the 5 word problems correctly", "At the end of the period, the student is able to compose a seven-to-ten sentence paragraph that observes unity and clarity", "At the end of the period, 80 of the class is able to solve the 5 words problems with 90 accuracy"},
        {"Test items that are a bit easy", "Test items that are moderate in difficulty", "Test items that are a bit difficulty", "Test items that every examinee gets correctly"},
        {"Hegel's", "Confucius", "Dewey's", "Teilhard de Chardin's"},
        {"Reclustered", "Integrated", "Overcrowded", "Innovative"},
        {"Abolition of private property", "Sense of social justice", "Individualistic concept of wealth", "Social ownership of some means of production"},
        {"Term of Specifications", "Table of Specifics", "Table of Specific Test Items", "Table of Specifications"},
        {"Explain to the pupils that smoking is bad for young people but not to adults", "Be tolerant about his behavior for it is only a manifestation of pakikisama", "Express disapproval in writing the unbecoming behavior of the teacher", "Talk with him about the importance of being consistent on actions and in the rules of discipline enforced in the classroom"},
        {"Portfolio assessment", "Journal entry", "Performance test", "Paper-and-pencil test"},
        {"The new political movement", "Political will to institute national reforms", "A sense of historical identity", "Chronic clan mentality"},
        {"gained mastery over that item", "performed very well against expectation", "found that test item was neither easy nor different", "were hard up in that item"},
        {"Brainstorming", "Role playing", "Buzz groups", "Inquiry group discussion"},
        {"makes effective use of cooperative learning techniques", "ask thought-provoking questions", "require uniform reports", "do inquiry teaching"},
        {"Ego and superego", "Ego", "Id", "Superego"},
        {"has eye contact with his students", "gives penetrating looks to his students", "looks at his students from head to toe when he expresses legitimate anger", "is aware of all actions and activities in the classroom"},
        {"Unit method", "Demonstration method", "Project method", "Integrative method"},
        {"passive recipients of knowledge", "rival of classmates in learning", "active partner in learning", "object of pedagogy"},
        {"Lithograph", "Hectograph", "Holograph", "Histogram"},
        {"Encourage the students to read books and magazines to improve their spelling", "Readily correcting the error upon utterance to distract the students flow of though", "Nothing students errors and dealing with them after the class activities", "Ignore the mistake, anyway she will commit the same error in the future"},
        {"questioner", "facilitator of learning", "guide on the side", "almighty teacher"},
        {"Sensorimotor", "Concrete operational", "Formal operational", "Pre-operational"},
        {"the computer lets the user win", "the computer program has menus", "a touch screen is used for input", "the computer program is easy to use"},
        {"Contiguity", "Similarity", "Frequency", "Contrast"},
        {"Full development of talents", "Athletic skills", "Respect for the dignity of persons", "Full development of physical powers"},
        {"When he refuses a pupil with a slight physical disability in class", "When he makes a farsighted pupil sit at the back", "When he makes a nearsighted pupil sit in front", "When he considers multiple intelligences in the choice of his teaching strategies"},
        {"Adolescence", "Middle childhood", "Infancy", "Early childhood"},
        {"Yes, rape is very serious act of aggression and is proportionate to killing", "Yes, it is done to defend her reputation", "Yes, this is the only time the woman can ably defend herself", "No, the killing is no longer self-defense because the rape is already over"},
        {"Point grading system", "Relative grading", "Mastery grading", "Grade contracting"},
        {"To question the illogical", "To criticize the unreasonable", "Think within the box", "Do outside-the-box thinking"},
        {"Exhibit", "Field trip", "Game", "Role playing"},
        {"Fishbone diagram", "Continuous scale", "Series of events chain", "Flow chart"},
        {"Doing mentally what was done just physically done", "Reasoning applied to specific example sets", "Classifying objects into different sets", "Imagining the sets necessary to complete an algebraic equation"},
        {"Judgment atmosphere", "Brainstorming technique", "Authoritarian teacher", "Convergent questions"},
        {"Computer-assisted instruction", "Games", "Cooperative learning", "Puzzle"},
        {"It is discovery-based learning", "It is student-centered", "It is school learning", "Students take part in planning lessons"},
        {"How should you present a report in class?", "What condition must be met for the reporting method to be effective?", "What steps are followed in class reporting?", "Was the student reporting well done? Support your answer"},
        {"Read a book then write a response", "Examine a statistical chart then write a response", "Watch a movie then write a response", "Go on a field trip then write a response"},
        {"Learners have stock knowledge of things based on background information and experiences", "There is no need to provide background information", "Children can be taught how to study", "Teachers must presume that learners know everything"},
        {"Model", "Slides", "Mock up", "Realia"},
        {"Label the parts of the butterfly on the paper", "Sequence the figures of the butterfly's life cycle", "Name the different stages a butterfly goes through", "Compare and contrast butterflies from moths"},
        {"Concept-level", "Facts and concept level", "Value-level", "Concepts and value level"},
        {"Evaluation", "Motivation", "Assignment", "Recitation"},
        {"Emotion", "Will", "Senses", "Reason"},
        {"Cooperative learning", "Integrative learning", "Adaptive instruction", "Independent study"},
        {"Concepts level", "Value level", "Experiential level", "Facts level"},
        {"Comparison", "Contrast", "Categorization", "Analogy"},
        {"science education", "skill education", "liberal education", "technical education"},
        {"No. He did not directly the death of his mother and the other passengers", "Yes, if he got the insurance money after the death", "Yes, he may have been directly interested only in the insurance money but indirectly as a foreseen consequence, he willed the death of all passengers", "No, if he refused to get the insurance money after the incident"},
        {"Symposium", "Panel discussion", "Debate", "Forum"},
        {"Concept maps", "Debates", "Manipulatives", "Deductive reasoning"},
        {"Twenty-five is the score that occurs least", "Twenty-five(25) is the score that occurs most", "Twenty-five is the average of the score distribution", "There is no score of 25"},
        {"The students must be highly intelligent", "The scores are concentrated on the right side of the normal distribution curve", "The scores are evenly distributed", "The scores are concentrated on the left side of the normal distribution curve"},
        {"Alternate response", "Short answer", "Completion", "Multiple choice"},
        {"Rousseau", "Kung-fu-tsu", "Dewey", "Hegel"},
        {"Journal", "Story map", "Venn diagram", "Semantic web"},
        {"Drawing inferences", "Recall", "Evaluation", "Stating generalization"},
        {"Use of interactive teaching strategies", "Observing continuing professional education", "Use of the latest instructional technology", "Study of the life of Filipino heroes"},
        {"Diagnostic test", "Summative test", "Performance test", "Formative test"},
        {"Cognitive psychology", "Humanistic psychology", "Behaviorist theory", "S-R theories"},
        {"Inability to read", "Inattention", "Poor health", "Inability to spell"},

    };
    char ProfessionalEducationAnswers[100] = {
    'B','C','D','B','C',
    'C','C','A','D','B',
    'B','D','D','B','A',
    'A','C','B','C','C',
    'C','D','B','D','C',
    'B','C','D','B','B',
    'C','C','B','D','D',
    'D','C','A','D','D',
    'B','C','D','B','B',
    'A','D','D','C','D',
    'D','D','C','C','D',
    'D','C','D','C','A',
    'C','D','B','C','A',
    'C','D','C','D','D',
    'C','D','B','C','D',
    'D','D','A','A','B',
    'D','D','A','D','A',
    'B','D','C','C','A',
    'C','B','D','D','C',
    'B','A','D','D','B',
};


//============================================================================================================

    char SpecializedSubjectQuestions[100][500] = {
    "Which of the following is NOT an operating system?",
    "A device driver is:",
    "What is NOT a way computers can output information?",
    "What led to the creation of the World Wide Web?",
    "Which statement is true regarding wireless connection?",
    "The creator of COBOL programming language and author of 'The Education of a Computer' was:",
    "If your computer has the IP address 128.96.10.123, and the subnet mask is set to 255.255.0.0, your computer will connect directly to all computers whose IP address starts with:",
    "A four bit number is given as 1001. Its 1's complement is:",
    "The binary representation of hexadecimal C3 is:",
    "The ASCII code is for information interchange by a binary code for:",
    "BCD numbers are obtained:",
    "The binary representation 100110 is numerically equivalent to:",
    "The application layer of the OSI model is the:",
    "For a logical circuit there are 'n' binary inputs. Then the number of different input combinations in the truth table is:",
    "Which of the following contains complete record of all activity that affected the contents of a database during a certain period of time?",
    "Multimedia software used for video editing is:",
    "Which of the following file formats is used for artwork?",
    "Which of the following is a language?",
    "To install multimedia facility into a computer, one does not essentially need:",
    "Hardware of computer means ______________.",
    "Do... Loop is an iterative statement because it:",
    "An array of controls can be populated by:",
    "Which type of project can a developer choose in the New Project dialog box?",
    "What does IDE stand for?",
    "Who is known as the father of computer?",
    "Who is credited with the creation of the first mechanical computer, the Difference Engine?",
    "Pascaline is known as:",
    "A megabyte, also known as MB, is:",
    "When we press a key on the keyboard, the computer system uses which of the following standards to convert the keystrokes into corresponding bits?",
    "Integrated circuits used in computers were developed using which of the following materials?",
    "Which of the following technology or component was used in second-generation computers?",
    "Which of the following keys is used to enter the BIOS setup when starting a computer?",
    "Who invented Java Programming?",
    "What is the extension of Java code files?",
    "What will happen if two threads of the same priority are called to be processed simultaneously?",
    "Which of these statements is incorrect?",
    "Unauthorized copying of software to be used for personal gain or personal backups is called:",
    "Like a virus, it is a self-replicating program that also propagates through computer networks.",
    "The terminal device often used in checking charge cards that offers both a limited keyboard input and visual output is the:",
    "The main distinguishing feature of fifth-generation computers is:",
    "The first attempt at using the punch card principle to do mathematical calculations led to the invention of:",
    "An opening on the motherboard of a system unit where an expansion card can be inserted is known as:",
    "The unit of the computer system that directs and supervises other units is called the:",
    "The central processing unit consists of:",
    "Viruses, worms, and Trojan horses are regarded as:",
    "The following are effects of file insecurities except:",
    "In binary, the decimal number 28 is represented as:",
    "Machine language instructions are expressions of:",
    "Which of the following commands would undo the last operation carried out in Microsoft Word?",
    "How many digits of the Data Network Identification Code (DNIC) identify Nigeria?",
    "E-mail addresses separate the username from the ISP using the '@' symbol.",
    "The LET statement in QBASIC:",
    "____________ are specially designed computer chips that reside inside other devices, such as your car or your electronic thermostat.",
    "In Boolean algebra, the OR operation is performed by which property?",
    "The expression for Absorption law is given by ________.",
    "The involution of A is equal to ________.",
    "A(A + B) = ?",
    "Complement of the expression A’B + CD’ is ________.",
    "How many gates would be required to implement the following Boolean expression before simplification? XY + X(X + Z) + Y(X + Z)",
    "Given that F = (A + B' + C)(D + E), which of the following represents the only correct expression for F'?",
    "What is the primary motivation for using Boolean algebra to simplify logic expressions?",
    "Which gate is known as the inverter?",
    "DeMorgan's Theorem states that (A + B)' is equal to:",
    "What is the purpose of the synchronized keyword in Java?",
    "Which interface is used to create a thread in Java?",
    "Which of the following is a valid access modifier in Java?",
    "Why is hyperparameter tuning an important step in training machine learning models, and what are some common techniques for hyperparameter optimization?",
    "How do transformers work, and why have they become so popular in natural language processing?",
    "Who is known for their work on Capsule Networks as an alternative to convolutional neural networks?",
    "A flowchart that outlines the main segments of a program.",
    "The action performed by a ___________ structure must eventually cause the loop to terminate.",
    "The operation represented by parallelograms.",
    "A technique that allows more than one program to be ready for execution and provides the ability to switch from one process to another.",
    "Processor wait ratio is given by ________.",
    "The number of tests required to break the Double DES algorithm are:",
    "Which of the following is defined as an attempt to steal, spy, damage or destroy computer systems, networks, or their associated information?",
    "Cyberspace was coined by ________.",
    "When you use the word _____ it means you are protecting your data from getting disclosed.",
    "Why are these 4 elements (confidentiality, integrity, authenticity & availability) considered fundamental?",
    "Which of these is not a proper method of maintaining confidentiality?",
    "______ is the latest technology that faces an extra challenge because of the CIA paradigm.",
    "Define Agile Scrum methodology.",
    "CASE stands for:",
    "Why do bugs and failures occur in software?",
    "Agile Software Development is based on which of the following types?",
    "Quality Management is known as _______.",
    "According to IBM research, “31% of projects are abandoned before they are completed, 53% exceed their cost projections by an average of 189 percent, and 94 projects are restarted for every 100 projects.” What is the significance of these figures?",
    "Among the given scientists/inventors, who is known as the father of Computer Graphics?",
    "Curves in computer graphics are primarily used for which of the following functions?",
    "How many axes do 3D graphics consist of?",
    "If the boundary is specified in a single color, and if the algorithm proceeds pixel by pixel until the boundary color is encountered, it is known as ____.",
    "The Cohen-Sutherland algorithm divides the region into how many spaces?",
    "Which of the following algorithms can be used to clip a polygon in 3D space?",
    "How is the line path on the polygon area for a 45° line?",
    "____________ is basically a form of pictorial presentation.",
    "The ____________ is a piece of equipment designed to make cartoons more realistic and enjoyable. It uses stacked panes of glass, each with different elements of the animation.",
    "___________ animation is used to animate things that are smaller than life size.",
    "Who is the father of animation?",
    "___________ is an emerging branch in computer science, which interprets means and methods of making computers think like human beings.",
    "A program that can retrieve files from the World Wide Web and render text, images, or sounds encoded in the files.",
};


    char SpecializedSubjectOptions[100][4][300] = {
    {"Linux", "Microsoft Vista", "Microsoft Word", "Mac OS X"},
    {"The person who delivers hardware", "The connector that allows you to attach the device", "A program that allows OS to communicate with the device", "The power supply for the device"},
    {"Printer", "Monitor", "MP3 Files", "Microphone"},
    {"The Internet", "Hypertext Markup Language", "Hypertext", "The Intranet"},
    {"Wireless cards are only built into computers", "All wireless connections have identical speed", "Wireless connections allow you to connect from varying distances", "There is only one type of wireless device"},
    {"Steve Jobs", "Dr. John Kemeny", "Dr. Ted Hoff", "Dr. Grace Hopper"},
    {"128", "128.96", "128.96.10", "128.96.10.12"},
    {"1001", "11001", "0110", "0101"},
    {"1111", "110011", "110001", "11000011"},
    {"Numbers only", "Alphabets only", "Alphanumeric and other common symbols", "None of these"},
    {"By converting decimal number to binary", "By converting decimal to octal", "When each decimal digit is represented by four-bit binary", "By converting binary to decimal"},
    {"The decimal representation 46", "The octal representation 46", "The hexadecimal representation 46", "The binary representation 26"},
    {"Seventh Layer", "Sixth Layer", "Fifth Layer", "Fourth Layer"},
    {"2n", "2/n", "2nd", "2(n+1)"},
    {"Master File", "Transaction File", "Report", "Query File"},
    {"Adobe Illustrator", "Adobe Photoshop", "Adobe Premier", "Macromedia Director"},
    {"GIF", "JPEG", "PNG", "MPEG"},
    {"QBE", "SQL", "QUEL", "C++"},
    {"Sound Card", "Speakers", "Mouse", "CD-ROM"},
    {"Paper used", "Plastic box", "Electronic Circuit and Devices", "Magnetic particle"},
    {"Select a block of statements to run.", "Runs the same block of statements repeatedly.", "Selects a block of statements and runs it repeatedly.", "All of the above."},
    {"Assigning existing controls to the array.", "Creating controls and assigning them to the array.", "Borrowing controls that will automatically assign them to the array.", "Both A and B"},
    {"Visual Basic Projects", "Visual C# Projects", "Visual C Projects", "All of the above"},
    {"Integrated Development Environment", "Integrated Design Environment", "Interior Development Environment", "Interior Design Environment"},
    {"Dennis Ritchie", "Bill Gates", "Charles Babbage", "James Gosling"},
    {"Blaise Pascal", "Garden Moore", "Bjarne Stroustrup", "Charles Babbage"},
    {"Adding Machine", "Multiplication Machine", "Division Machine", "Difference Engine"},
    {"512 Megabytes", "1024 Megabytes", "1000 Kilobytes", "1024 Kilobytes"},
    {"ANSI", "ANSII", "ISO", "EBCDIC"},
    {"Silver", "Copper", "Gold", "Silicon"},
    {"Transistors", "Vacuum Tubes", "Integrated Circuits", "Semiconductors"},
    {"ESC", "F2", "F8", "F10"},
    {"Guido van Rossum", "Dennis Ritchie", "James Gosling", "Bjarne Stroustrup"},
    {".js", ".java", ".class", ".jav"},
    {"Anyone will be executed first lexographically", "Both of them will be executed simultaneously", "None of them will be executed", "It is dependent on the operating system"},
    {"By multithreading CPU idle time is minimized, and we can take maximum use of it", "By multitasking CPU idle time is minimized, and we can take maximum use of it", "Two threads in Java can have the same priority", "A thread can exist only in two states, running and blocked"},
    {"Program looting", "Program thievery", "Data snatching", "Software piracy"},
    {"Phishing scam", "Worm", "Spyware", "Cracker"},
    {"Audio response unit", "Video display terminal", "Pas terminal", "Intelligent terminal"},
    {"Liberal use of microprocessors", "Artificial intelligence", "Extremely low cost", "Versatility"},
    {"Napier's bone", "Abacus", "Charles Babbage Difference Engine", "Charles Babbage Analytical Engine"},
    {"Extension Board", "Extension Box", "Expansion Port", "Expansion Slot"},
    {"Accumulator", "Arithmetic Logic Unit", "Control Unit", "Memory Unit"},
    {"Input, output and processing", "Control unit, primary storage and secondary storage", "Control unit, arithmetic logic unit and primary storage", "Control unit, processing and primary storage"},
    {"Application software", "Malicious software", "System software", "Utility software"},
    {"Data over-writing", "Data loss", "Data use", "File corruption"},
    {"1001", "11001", "11101", "11100"},
    {"ASCII", "Binary", "Editor", "Hexadecimal"},
    {"Ctrl + Z", "Ctrl + Y", "Ctrl + W", "Ctrl + U"},
    {"First six", "First four", "First five", "First three"},
    {"&", "#", "@", "%"},
    {"Allows users to assign a value to a variable", "Checks the errors within the code", "Allows users to enter data into the computer from outside the program", "Allows users to enter data into the computer during program execution"},
    {"Servers", "Embedded computers", "Robotic computers", "Mainframes"},
    {"Associative properties", "Commutative properties", "Distributive Properties", "All of the mentioned"},
    {"A + AB = A", "A + AB = B", "AB + AA", "A + B = B + A"},
    {"A", "A'", "1", "0"},
    {"AB", "1", "(1 + AB)", "A"},
    {"1", "0", "AB", "AB'"},
    {"(A' + B)(C' + B)", "(A + B'')(C' + D)", "(A' + B)(C' + D)", "(A + B’)(C + D’)"},
    {"1", "2", "4", "5"},
    {"F' = A'BC' + D' + E'", "F' = AB'C + DE", "F' = (A' + B + C')(D' + E')", "F' = A'BC' + D'E'"},
    {"It may make it easier to understand the overall function of the circuit", "It may reduce the number of gates", "It may reduce the number of inputs required", "All of the above"},
    {"AND", "OR", "NOT", "XOR"},
    {"A' + B'", "A' . B'", "A + B", "A . B"},
    {"To define constant", "To create an object", "To control access to shared resources in a multithreaded environment", "To inherit a class"},
    {"Runnable", "Threadable", "Startable", "Process"},
    {"Main", "Class", "Public", "int"},
    {"Hyperparameter tuning is important because it reduces the size of the dataset.", "Hyperparameter tuning prevents overfitting.", "Hyperparameter tuning allows finding the best combination of hyperparameters that maximize model performance. Common techniques include grid search, random search, and Bayesian optimization.", "Hyperparameter tuning is not important."},
    {"Transformers work by using convolutional layers to process sequential data.", "Transformers work by using recurrent layers to process sequential data.", "Transformers work by using self-attention mechanisms to weigh the importance of different parts of the input sequence, capturing long-range dependencies and parallelizing computation, making them effective for NLP.", "Transformers do not work for natural language processing."},
    {"Andrew Ng", "Geoffrey Hinton", "Yann LeCun", "Yoshua Bengio"},
    {"Queue", "Macro", "Micro", "Union"},
    {"Sequence", "Case", "Repetition", "Process"},
    {"Input/Output", "Assignment", "Comparison", "Conditions"},
    {"Multitasking", "Multiprocessing", "Multiprogramming", "Multithreading"},
    {"w = b/e + b", "w = b/e - b", "# = b/e - b", "# = b/e + b"},
    {"2112", "2111", "2128", "2119"},
    {"Cyber attack", "Computer security", "Cryptography", "Digital Hacking"},
    {"Richard Stallman", "William Gibson", "Andrew Tannenbaum", "Scott Fahlman"},
    {"Confidentiality", "Integrity", "Authentication", "Availability"},
    {"They help understand hacking better", "They are key elements to a security breach", "They help understand security and its components better", "They help to understand cyber-crime better"},
    {"Biometric verification", "ID and password based verification", "2-factor authentication", "Switching off the phone"},
    {"Big Data", "Database system", "Cloud Storages", "Smart Dust"},
    {"Project management that emphasizes incremental progress", "Project management that emphasizes decremental progress", "Project management that emphasizes neutral progress", "Project management that emphasizes no progress"},
    {"Computer-Aided Software Engineering", "Control Aided Science and Engineering", "Cost Aided System Experiments", "None of the mentioned"},
    {"Because of Developers", "Because of companies", "Because of both companies and Developers", "None of the mentioned"},
    {"Iterative Development", "Incremental Development", "Both Incremental and Iterative Development", "Linear Development"},
    {"SQI", "SQA", "SQM", "SQA and SQM"},
    {"Lack of software ethics and understanding", "Management issues in the company", "Lack of adequate training", "All of the mentioned"},
    {"Nikola Tesla", "Ivan Sutherland", "Ada Lovelace", "Marie Curie"},
    {"To draw different types of objects onto the screen", "Zooming out a picture", "Copying a picture", "Zooming in a picture"},
    {"Two axes", "Three axes", "Five axes", "One axis"},
    {"Parallel curve algorithm", "Flood-fill algorithm", "Scan-line fill algorithm", "Boundary-fill algorithm"},
    {"9", "8", "7", "6"},
    {"Vatti Clipping Algorithm", "Polygon in 3D space cannot be clipped", "Weiler Atherton Algorithm", "Greiner Hormann Clipping Algorithm"},
    {"Vertical", "Horizontal", "Centered", "None of the mentioned"},
    {"Photography", "Animation", "Drawing", "Creativity"},
    {"Multiplane camera", "VR", "Thaumatrope", "Phenakistoscope"},
    {"Immersive", "Claymotion", "Stopmotion", "Augmented"},
    {"Walt Disney", "J. Stuart Blackton", "William Horner", "J.A. Ferdinand Plateau"},
    {"Block Chain", "VR", "AI", "Cloud Computing"},
};


char SpecializedSubjectAnswers[100] = {
    'C','C','C','C','C',
    'D','B','C','D','C',
    'C','B','A','C','B',
    'C','B','D','C','C',
    'B','D','D','A','C',
    'C','A','D','A','D',
    'A','B','B','B','D',
    'D','A','B','B','D',
    'D','C','C','B','C',
    'D','B','A','B','D',
    'A','A','D','B','B',
    'D','D','D','C','B',
    'C','A','C','C','C',
    'B','B','C','A','D',
    'A','B','A','B','A',
    'C','D','A','A','A',
    'C','C','B','D','B',
    'A','B','D','A','C',
    'C','C','A','C','B',
    'C','C','A','C','B'
};

//=================================================================================================================================================================================

    while (1) {
        printf("\n=== LPT Examination ===\n");
        showLatestExam(currentUser);
        printf("4. Log out\n");
        printf("Choose a category: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                takeExam("General Education", GeneralEducationQuestions, GeneralEducationOptions, GeneralEducationAnswers, 100);
                break;
            case 2:
                takeExam("Professional Education", ProfessionalEducationQuestions, ProfessionalEducationOptions, ProfessionalEducationAnswers, 100);
                break;
            case 3:
                takeExam("Specialized Subject", SpecializedSubjectQuestions, SpecializedSubjectOptions, SpecializedSubjectAnswers, 100);
                break;
            case 4:
                printf("Logged out Successfully\n");
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
