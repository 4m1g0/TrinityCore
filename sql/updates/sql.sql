
DROP TABLE IF EXISTS `spam_reports`;
CREATE TABLE `spam_reports` (
  `guid` int(10) unsigned NOT NULL,
  `reporter` int(10) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `languaje` tinyint(8) unsigned NOT NULL,
  `mailOrType` int(10) unsigned NOT NULL DEFAULT '0',
  `text` longtext COLLATE utf8_bin
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

DELETE FROM trinity_string WHERE entry IN (12002, 12003);
INSERT INTO trinity_string (entry, content_default) VALUES (12002, 'Chat report from %s (guid: %u). Languaje: %s. Chat type: %s, %s');
INSERT INTO trinity_string (entry, content_default) VALUES (12003, 'Mail report from %s (guid: %u). Mail id: %u. Text: [%s]');
