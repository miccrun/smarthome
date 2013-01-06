
DROP TABLE IF EXISTS `monitor`;
CREATE TABLE IF NOT EXISTS `monitor` (
    `monitor_id` int unsigned NOT NULL AUTO_INCREMENT,
    `temperature` tinyint NOT NULL DEFAULT '0',
    `humidity` tinyint NOT NULL DEFAULT '0',
    `heat_index` tinyint NOT NULL DEFAULT '0',
    `valid` tinyint NOT NULL DEFAULT '1',
    `update_time` int unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY (`monitor_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;

