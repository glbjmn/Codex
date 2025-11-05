#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
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
    fprintf(fp, "%s \n Score: %d/%d \n Percent Score: %.2f%% \n Date: %s \n Taken by: %s \n",
            subject, score, total, percent, formattedDate, username);

    fclose(fp);
}


//=================================================================================================================================================================================


void takeExam(const char *subject, char questions[][500], char options[][4][200], char answers[], int totalQuestions) {
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

    char line[512];
    char latestGE[512] = "";
    char latestPE[512] = "";
    char latestSS[512] = "";

    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, username)) {
            if (strstr(line, "General Education"))
                strcpy(latestGE, line);
            else if (strstr(line, "Professional Education"))
                strcpy(latestPE, line);
            else if (strstr(line, "Specialized Subject"))
                strcpy(latestSS, line);
        }
    }

    fclose(fp);

    printf("\n=== Exam for Examinee %s ===\n", username);

    if (strlen(latestGE) > 0)
        printf("1. %s", latestGE);
    else
        printf("1. General Education.\n");

    if (strlen(latestPE) > 0)
        printf("2. %s", latestPE);
    else
        printf("2. Professional Education.\n");

    if (strlen(latestSS) > 0)
        printf("3. %s", latestSS);
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

    char GeneralEducationQuestions[10][500] = {
        "What is 5 + 3?", 
        "What is 9 - 4?", 
        "What is 6 x 7?", 
        "What is 20 / 4?",
        "What is the square root of 81?", 
        "What is 12 + 15?", 
        "What is 100 - 25?",
        "What is 8 x 8?", 
        "What is 10 / 2?", 
        "What is 9 + 10?"
    };
    char GeneralEducationOptions[10][4][200] = {
        {"6","7","8","9"},
        {"3","5","6","7"},
        {"42","49","36","56"},
        {"6","5","4","3"},
        {"7","8","9","10"}, 
        {"26","27","28","29"}, 
        {"65","70","75","80"}, 
        {"56","64","72","81"},
        {"4","5","6","7"}, 
        {"18","19","20","21"}
    };
    char GeneralEducationAnswers[10] = {'C','B','A','B','C','B','C','B','B','B'};

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
    char ProfessionalEducationOptions[100][4][200] = {
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
    char ProfessionalEducationAnswers[100] = {'B','C','D','B','C','C','C','A','D','B','B','D','D','B','A','A','C','B','C','C','C','D','B','D','C','B','C','D','B','B','C','C','B','D','D','D','C','A','D','D','B','C','D','B','B','A','D','D','C','D','D','D','C','C','D','D','C','D','C','A','C','D','B','C','A','C','D','C','D','D','C','D','B','C','D','D','A','A','B','D','D','A','D','A','B','D','C','C','A','C','B','D','D','C','B','A','D','D','B','B',};

//============================================================================================================

    char SpecializedSubjectQuestions[31][500] = {
        "Which of the following is NOT an operating system?",
        "A device driver is?",
        "What is NOT a way computers can output information?",
        "What led to the creation of the World Wide Web?",
        "Which statement is true regarding wireless connection?",
        "The creator of COBOL programming language and author of 'The Education of a Computer' was:",
        "If your computer has the IP address 128.96.10.123, and the subnet mask is set to 255.255.0.0, your computer will connect directly to all computers whose IP address starts with",
        "A four bit number is given as 1001 Its-1's complement is",
        "The binary representation of hexadecimal C3 is",
        "The ASCII code is for information interchange by a binary code for",
        "BCD numbers are obtained",
        "The binary representation 100110 is numerically equivalent to",
        "The application layer of the OSI model is the",
        "For a logical circuit there are 'n' binary inputs. Then the number of different input combinations in the truth table is",
        "Which of the following contains complete record of all activity that affected the contents of a database during a certain period of time",
        "Multimedia S/W used for video editing is",
        "Which of the following file formats is used for art work",
        "Which of the following is a programming language",
        "To install multimedia facility into a computer, one does not essentially need",
        "Hardware of computer means ____________.",
        "Do... Loop is an iterative statement because it;",
        "An array of controls can be populated by?",
        "Which type of project can a developer choose in the New Project dialog box?",
        "What does IDE stand for?",
        "Who is known as the father of computer?",
        "Who is credited with the creation of the first mechanical computer, the Difference Engine?",
        "Pascaline is known as:",
        "A megabyte, also known as a MB",
        "When we press a key on the keyboard, the computer system uses which of the following standards to convert the keystrokes into corresponding bits?",
        "Integrated circuits used in computers are made of what material?",
        "Which of the following technology or component was used in second-generation computers?",
    };

    char SpecializedSubjectOptions[31][4][200] = {
        {"Linux","Microsoft Vista","Microsoft Word","Mac OS X"},
        {"The person who delivers hardware","The connector that allows you to attach the device","A program that allows OS to communicate with the device","The power supply for the device"},
        {"Printer","Monitor","MP3 Files","Microphone"},
        {"The Internet","Hypertext Markup Language","Hypertext","The Intranet"},
        {"Wireless cards are only built into computers","All wireless connections have identical speed","Wireless connections allow you to connect from varying distances","There is only one type of wireless device"},
        {"Steve Jobs","Dr. John Kemeny","Dr. Ted Hoff","Dr. Grace Hopper"},
        {"128","128.96","128.96.10","128.96.10.12"},
        {"1001", "11001", "0110", "0101"},
        {"1111", "110011", "110001", "11000011"},
        {"Numbers only", "Alphabets only", "Alphanumeric and other common symbols", "None of these"},
        {"By converting decimal number to binary", "By converting decimal to octal", "When each decimal digit is represented by four-bit binary", "By converting binary to decimal"},
        {"The decimal representation 46", "The octal representation 46", "The hexadecimal representation 46", "The binary representation 26"},
        {"Seventh Layer", "Sixth Layer","Fifth Layer","Fourth Layer"},
        {"2n", "2/n","2nd", "2(n+1)"},
        {"Master File", "Transaction File", "Report", "Quiert File"},
        {"Adobe Illustrator","Adobe Photoshop","Adobe Premier", "Macromedia Director"},
        {"GIF", "JPEG", "PNG", "MPEG"},
        {"QBE", "SQL","QUEL", "C++"},
        {"Sound Card", "Speakers", "Mouse", "CD-ROM"},
        {"Paper used", "Plastic box", "Electronic Circuit and Devices", "Magnetic particle"},   
        {"Select a block of statements to run.", "Runs the same block of statements repeatedly.", "Selects a block of statements and runs it repeatedly.", "All of the above."},
        {"Assigning existing controls to the array.","Creating controls and assigning them to the array.", "Borrowing controls that will automatically assign them to the array.", "Both A and B"},
        {"Visual Basic Projects" ,"Visual C# Projects","Visual C Projects", "All of the above"},
        {"Integrated Development Environment", "Integrated Design Environment", "Interior Development Environment", "Interior Design Environment"},
        {"Dennis Ritchie", "Bill Gates","Charles Babbage", "James Gosling"},
        {"Blaise Pascal", "Garden Moore", "Bjarne Stroustrup", "Charles Babbage"},
        {"Adding Machine", "Multiplication Machine", "Division Machine", "Difference Engine"},
        {"512 Megabytes", "1024 Megabytes", "1000 Kilobytes", "1024 Kilobytes"},
        {"ANSI", "ANSII","ISO", "EBCDIC"},
        {"Silver","Copper", "Gold", "Silicon"},
        {"Transistors", "Vacuum Tubes", "Integrated Circuits", "Semiconductors"},
    };

    char SpecializedSubjectAnswers[31] = {'C','C','C','C','C','D','B','C','D','C','C','B','A','C','B','C','B','D','C','C','B','D','D','A','C','C','A','D','A','D','A'};

//=================================================================================================================================================================================

    while (1) {
        printf("\n=== LPT Examination ===\n");
        showLatestExam(currentUser); 
        printf("4. Log out\n");
        printf("Choose a category: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                takeExam("General Education", GeneralEducationQuestions, GeneralEducationOptions, GeneralEducationAnswers, 10);
                break;
            case 2:
                takeExam("Professional Education", ProfessionalEducationQuestions, ProfessionalEducationOptions, ProfessionalEducationAnswers, 100);
                break;
            case 3:
                takeExam("Specialized Subject", SpecializedSubjectQuestions, SpecializedSubjectOptions, SpecializedSubjectAnswers, 31);
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