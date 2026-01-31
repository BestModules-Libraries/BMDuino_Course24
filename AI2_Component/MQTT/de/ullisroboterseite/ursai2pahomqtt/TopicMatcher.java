package de.ullisroboterseite.ursai2pahomqtt;

/**
 * topicmatcher
 */
public class TopicMatcher {
    // Quelle: https://github.com/iosphere/mosquitto
    /*
     * Die Methode wurde Mosquitto entnommen. Die ursprünliche C-Methode heißt
     * mosquitto_topic_matches_sub2. Sie wurde portiert und für Java aufbereitet.
     * Dabei wurde möglichst wenig verändert. Damit der ursrünliche Code
     * funktionert, wurde z.B. an die Strings ein '\0' angehängt. Damit klappen dann
     * auch die Abfragen auf '\0' als String-Ende-Zeichen in C.
     */

    public static final int MOSQ_MATCH = 0;
    public static final int MOSQ_NOMATCH = 1;
    public static final int MOSQ_ERR_INVAL = 2;

    /* Does a topic match a subscription? */
    // sub darf WildCards enthalten, topic nicht
    static int topicMatchesSubscription(String sub, String topic) {
        int spos;

        if (sub != null)
            if (!sub.isEmpty())
                sub += '\0';
        if (topic != null)
            if (!topic.isEmpty())
                topic += '\0';

        int subPtr = 0;
        int topicPtr = 0;

        if (sub == null || topic == null || sub.isEmpty() || topic.isEmpty()) {
            return MOSQ_ERR_INVAL;
        }

        if ((sub.charAt(subPtr + 0) == '$' && topic.charAt(topicPtr + 0) != '$')
                || (topic.charAt(topicPtr + 0) == '$' && sub.charAt(subPtr + 0) != '$')) {

            return MOSQ_NOMATCH;
        }

        spos = 0;

        while (sub.charAt(subPtr + 0) != 0) {
            if (topic.charAt(topicPtr + 0) == '+' || topic.charAt(topicPtr + 0) == '#') {
                return MOSQ_ERR_INVAL;
            }
            if (sub.charAt(subPtr + 0) != topic.charAt(topicPtr + 0)
                    || topic.charAt(topicPtr + 0) == 0) { /* Check for wildcard matches */
                if (sub.charAt(subPtr + 0) == '+') {
                    /* Check for bad "+foo" or "a/+foo" subscription */
                    if (spos > 0 && sub.charAt(subPtr + -1) != '/') {
                        return MOSQ_ERR_INVAL;
                    }
                    /* Check for bad "foo+" or "foo+/a" subscription */
                    if (sub.charAt(subPtr + 1) != 0 && sub.charAt(subPtr + 1) != '/') {
                        return MOSQ_ERR_INVAL;
                    }
                    spos++;
                    subPtr++;
                    while (topic.charAt(topicPtr + 0) != 0 && topic.charAt(topicPtr + 0) != '/') {
                        topicPtr++;
                    }
                    if (topic.charAt(topicPtr + 0) == 0 && sub.charAt(subPtr + 0) == 0) {
                        return MOSQ_MATCH;
                    }
                } else if (sub.charAt(subPtr + 0) == '#') {
                    /* Check for bad "foo#" subscription */
                    if (spos > 0 && sub.charAt(subPtr + -1) != '/') {
                        return MOSQ_ERR_INVAL;
                    }
                    /* Check for # not the final character of the sub, e.g. "#foo" */
                    if (sub.charAt(subPtr + 1) != 0) {
                        return MOSQ_ERR_INVAL;
                    } else {
                        return MOSQ_MATCH;
                    }
                } else {
                    /* Check for e.g. foo/bar matching foo/+/# */
                    if (topic.charAt(topicPtr + 0) == 0 && spos > 0 && sub.charAt(subPtr + -1) == '+'
                            && sub.charAt(subPtr + 0) == '/' && sub.charAt(subPtr + 1) == '#') {
                        return MOSQ_MATCH;
                    }

                    /* There is no match at this point, but is the sub invalid? */
                    while (sub.charAt(subPtr + 0) != 0) {
                        if (sub.charAt(subPtr + 0) == '#' && sub.charAt(subPtr + 1) != 0) {
                            return MOSQ_ERR_INVAL;
                        }
                        spos++;
                        subPtr++;
                    }

                    /* Valid input, but no match */
                    return MOSQ_NOMATCH;
                }
            } else {
                /* sub.charAt(subPtr +spos] == topic.charAt(topicPtr +tpos] */
                if (topic.charAt(topicPtr + 1) == 0) {
                    /* Check for e.g. foo matching foo/# */
                    if (sub.charAt(subPtr + 1) == '/' && sub.charAt(subPtr + 2) == '#' && sub.charAt(subPtr + 3) == 0) {
                        return MOSQ_MATCH;
                    }
                }
                spos++;
                subPtr++;
                topicPtr++;
                if (sub.charAt(subPtr + 0) == 0 && topic.charAt(topicPtr + 0) == 0) {
                    return MOSQ_MATCH;
                } else if (topic.charAt(topicPtr + 0) == 0 && sub.charAt(subPtr + 0) == '+'
                        && sub.charAt(subPtr + 1) == 0) {
                    if (spos > 0 && sub.charAt(subPtr - 1) != '/') {
                        return MOSQ_ERR_INVAL;
                    }
                    spos++;
                    subPtr++;
                    return MOSQ_MATCH;
                }
            }
        } // while

        if ((topic.charAt(topicPtr + 0) != 0 || sub.charAt(subPtr + 0) != 0)) {
            return MOSQ_NOMATCH;
        }

        return MOSQ_NOMATCH;
    } // mosquitto_topic_matches_sub
}