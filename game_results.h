#pragma once

/*
* inner array in game results representing one test result
 */
class one_test_result {
    bool pass;
    std::string name;
    std::string errors_in_test;

public:

    one_test_result(const bool pass, const std::string &name, const std::string &errors_in_test) {
        this->pass = pass;
        this->name = name;
        this->errors_in_test = errors_in_test;
    }

    one_test_result() {
        this->pass = false;
        this->name = "";
        this->errors_in_test = "";
    }

    [[nodiscard]] bool get_pass() const {
        return this->pass;
    }

    [[nodiscard]] std::string get_name() const {
        return this->name;
    }

    [[nodiscard]] std::string get_errors_in_test() const {
        return this->errors_in_test;
    }

    void set_game_name(const string &new_name) {
        this->name = new_name;
    }

    void set_pass(const bool is_pass) {
        this->pass = is_pass;
    }

    void set_errors_in_test(const string &new_test_result) {
        errors_in_test = new_test_result;
    }
};

/*
 * type that define game results
 */
class one_device_results {
private:

    std::string game_name;
    std::string platform_name;
    List<one_test_result> stages_res;

public:

    one_device_results() {
        this->game_name = "";
        this->platform_name = "";
        this->stages_res = List<one_test_result>();
    };

    std::string get_game_name() {
        return game_name;
    }

    std::string get_platform_name() {
        return platform_name;
    }

    List<one_test_result> get_stages_res() {
        return stages_res;
    }

    void set_game_name(const_string &new_name) {
        game_name = new_name;
    }

    void set_platform_name(const_string &new_platform_name) {
        platform_name = new_platform_name;
    }

    void set_stages_res(const one_test_result &new_test_result, const counter_v position) const {
        stages_res.getElement(position) = new_test_result;
    }

    /*
     * method that clears array
     */
    void clear(const counter_v size) {
        for (int i = 0; i < size; i++) {
            stages_res.getElement(i).~one_test_result();
        }
        stages_res.~List();
    }

    [[nodiscard]] counter_v size() const {
        return this->stages_res.getSize();
    }

    one_device_results &operator=(const char *by) {
        return *this;
    }
};

inline one_test_result get_empty() {
    one_test_result empty = {};
    return empty;
}

inline void enter_data(one_test_result to_input, const bool is_pass, const string &test_name, const string &errors) {
    to_input.set_pass(is_pass);
    to_input.set_game_name(test_name);
    to_input.set_errors_in_test(errors);
}
