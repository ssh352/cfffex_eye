<?php

define( 'DS', DIRECTORY_SEPARATOR );
define( 'FILE_DIR', dirname( __FILE__ )) ;

$opt = CmdHelper::getopt($argc, $argv);
//print_r($opt); die();

if (file_exists($opt['dst_dir']) && !is_dir($opt['dst_dir'])) {
    echo $opt['dst_dir'] . " exist, but not a dir\n";
    die();
}
FileHelper::MakeDir($opt['dst_dir']);

$filters = array('filters'=>array(), 'header'=>array(), 'source'=>array());
FilterParser::parse_dir_array($opt['dst_dir'], $opt['source'], $filters);
$filters['header'] = array_unique($filters['header']);
$filters['source'] = array_unique($filters['source']);
//print_r($filters); die();

$output_file = $opt['dst_dir'] . DS . $opt['project'];
$filter_content = FilterParser::render(FILE_DIR.DS."linux_vcxproj_filters.php", $filters);
FileHelper::FilePutContents($output_file.'.vcxproj.filters', $filter_content);

//$opt['include'] = $opt['source'].';' . $opt['include'];
//$opt['include'] = str_replace('/', '\\', $opt['include']);

$vcxproj_content = FilterParser::render(FILE_DIR.DS."linux_vcxproj.php",
    array_merge($opt, $filters));
FileHelper::FilePutContents($output_file.'.vcxproj', $vcxproj_content);

echo "generate  " . $output_file.'.vcxproj.filters' . "\n" .
     "          " . $output_file.'.vcxproj' . "       success\n";


class CmdHelper {
    public static $cmd_opt = array(
        'project'       => array('vcproj project name'),
        'dst_dir'       => array('vcproj output dir'),
        'out_dir'       => array('exe output dir'),
        'source'        => array('source file dir list, eg: dir1 dir2 dir3'),
        'include'       => array('depend include dir,  eg: dir1 dir2 dir3'),
        'library_dir'   => array('depend library dir,  eg: dir1 dir2 dir3'),
        'library_release'       => array('depend library,  eg: libboost-system.lib'),
        'library_debug'       => array('depend library,  eg: libboost-systemd.lib'),
        'macros'        => array('define macros, eg: WIN32'),
        'cc_flag'        => array('define macros, eg: WIN32'),
        'link_flag'        => array('define macros, eg: WIN32'),
        'help'          => array('show help info'),
    );
    public static function getopt($argc, $argv) {
        if ($argc < 2) {
            echo "bad argc\n";
            self::usage($argc, $argv);
            die();
        }

        $small_keys = array();
        $opt = self::$cmd_opt;
        foreach ($opt as $k => &$v) {
            $small_keys[$v[0]] = $k;
            $v = '';
        }

        $current_key = '';
        for ($i = 1; $i < count($argv); ++$i) {
            $v = $argv[$i];
            if ($v[0] == '-' && $v[1] == '-') {
                $kk = substr($v, 2, strpos($v, '=') - 2);
                $vv = substr($v, strpos($v, '=') + 1);
                if ($kk == '--help') {
                    echo "bad argc\n";
                    self::usage($argc, $argv);
                    die();
                }
                @$opt[$kk] = $vv;
            }
        }
        if (empty($opt['source']) || empty($opt['project'])) {
            echo "source or project is empty\n";
            self::usage($argc, $argv);
            die();
        }

        $dst_dir = (empty($opt['dst_dir'])) ? '.' : $opt['dst_dir'];
        $out_dir = '$(SolutionDir)';
        if (!empty($opt['out_dir'])) {
            $out_dir = $opt['out_dir'];
            if ($out_dir[1] != '.') {  //relative path
                $out_dir = '$(SolutionDir)' . '/' . $out_dir;
            }
        }

        $ret = array(
            'project'       => $opt['project'],
            'dst_dir'       => $dst_dir,
            'out_dir'       => str_replace('/', '\\', $out_dir),
            'source'        => $opt['source'],
            'include'       => $opt['include'],
            'library_dir'   => $opt['library_dir'],
            'library_release'       => $opt['library_release'],
            'library_debug'       => $opt['library_debug'],
            'macros'        => $opt['macros'],
            'cc_flag'        => $opt['cc_flag'],
            'link_flag'        => $opt['link_flag'],
        );
        foreach ($ret as $key => &$value) {
            $value = str_replace(',', ';', $value);
        }
        $ret['cc_flag'] = str_replace(';', ' ', $ret['cc_flag']);
        $ret['link_flag'] = str_replace(';', ' ', $ret['link_flag']);    
        print_r($ret);
        return $ret;
    }

    public static function usage($argc, $argv) {
        echo $argv[0] . " options\n";
        foreach (self::$cmd_opt as $k => $v) {
            echo "     --{$k}, ".$v[0]."\n";
        }
    }
}

class FilterParser {
    public static function parse_dir_array($dst_dir, $source_dirs, &$filters) {
        $source_dirs = explode(';', $source_dirs);
        foreach ($source_dirs as $key => $source_dir) {
            $last_char = $source_dir[strlen($source_dir) - 1];
            if ($last_char == '/' || $last_char == '\\') {
                $source_dir = substr($source_dir, 0, strlen($source_dir) - 1);
            }
            self::init_dir_filter($dst_dir, $source_dir, $filters);
            //var_dump($source_dir);
            self::parse_dir($dst_dir, $source_dir, $filters);
        }
    }

    public static function init_dir_filter($dst_dir, $source_dir, &$filters)
    {
        $dir = $dst_dir . DS . $source_dir; //var_dump($dir);
        if (!file_exists($dir) || !is_dir($dir)) {
            return;
        }
        $filtername = str_replace('\\', '/', $source_dir);
        $begin = 0;
        $pos = strpos($filtername, '/', $begin);
        $filter = '';
        while($pos !== false) {
            $one_dir = substr($filtername, $begin, $pos - $begin);
            //var_dump($one_dir);
            if ($one_dir != '.')
            {
                $filter = (empty($filter)) ? $one_dir : $filter . '\\' . $one_dir;
                if (!isset($filters['filters'][$filter])) {
                    $filters['filters'][$filter] = array();
                }
            }
            $begin   = $pos + 1;
            $pos     = strpos($filtername, '/', $begin);
        }
    }

    public static function parse_dir($dst_dir, $source_dir, &$filters)
    {
        if ($source_dir == '\\' || $source_dir == '/') {
            return;
        }
        $dir = $dst_dir . DS . $source_dir;
        $handle = NULL;
        if (!file_exists($dir) || false === ($handle = opendir($dir))) {
            return;
        }

        $filter_name = self::parse_filter_name($source_dir);
        //var_dump($filter_name);
        $filters['filters'][$filter_name] = array();
        while(false !== ($file = readdir($handle)))
        {
            if ($file === '.' || $file === '..') {
                continue;
            }

            $full_path = $dir . DS . $file;
            $full_filter = $source_dir . DS. $file;
            if (is_dir($full_path)) {
                self::parse_dir($dst_dir, $full_filter, $filters);
                continue;
            }

            $full_file_path = str_replace('/', '\\', $full_filter);
            $file_type = substr($file, strrpos($file, '.') + 1);
            if (in_array($file_type, array('h', 'hpp', 'hxx', 'ihh'))) {
                $filters['filters'][$filter_name]['header'][] = $full_file_path;
                $filters['header'][] = $full_file_path;
            } else if (in_array($file_type, array('c', 'cpp', 'cc', 'cxx'))) {
                $filters['filters'][$filter_name]['source'][] = $full_file_path;
                $filters['source'][] = $full_file_path;
            } else {
                echo "not recognized file type $file_type\n";
            }
        }
        closedir($handle);
    }

    public static function parse_filter_name($dir_name) {
        $str = str_replace('/', '\\', $dir_name);
        if ($str[0] == '.' && $str[1] == '\\') {
            $str = substr($str, 2);
        }
        return $str;
    }

    public static function render($file, $data = null)
    {
        if(is_array($data))
            extract($data,EXTR_PREFIX_SAME,'data');
        else
            $data=$data;

        ob_start();
        ob_implicit_flush(false);
        require($file);
        return ob_get_clean();
    }
}


class CGuidManager
{
    public static function GetGuid()
    {
        list($usec, $sec) = explode(" ", microtime());
        $guid = date("YmdHis", $sec) . self::GetStrUsec($usec) . self::GetRandomString(4);
        return $guid;
    }

    public static function GetStrUsec($usec)
    {
        $strTemp = (string)((float)$usec*100000000);
        while(strlen($strTemp)<8)
        {
            $strTemp = '0'.$strTemp;
        }
        return $strTemp;
    }

    public static function GetRandomString($len=10)
    {

        $chars = array(
            "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
            "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v",
            "w", "x", "y", "z", "A", "B", "C", "D", "E", "F", "G",
            "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R",
            "S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2",
            "3", "4", "5", "6", "7", "8", "9",
        );
        $charsLen = count($chars) - 1;
        shuffle($chars);

        $output = "";
        for ($i=0; $i<$len; $i++)
        {
            $output .= $chars[mt_rand(0, $charsLen)];
        }

        return $output;
    }
}

class FileHelper
{
    public static function FilePutContents($filename, $content)
    {
        self::MakeDir(dirname($filename));
        file_put_contents($filename, $content);
    }
    public static function MakeDir($dir) {
        if (!is_dir(dirname($dir))) {
            self::MakeDir(dirname($dir));
        }
        if (!is_dir($dir)) {
            mkdir($dir, 0755);
        }
    }
    public static function Remove($srcfile, $destfile)
    {
        $pos = strrpos($file, '/');
        self::MakeDir(substr($destfile, 0, $pos));
        rename($destfile, $srcfile);
    }
}