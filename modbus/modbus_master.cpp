#include "modbus_master.h"

using namespace std;

int Modbus_Master::Register_Rule(unsigned char funcode, unsigned short startaddr, unsigned short regnum, unsigned short interval)
{
    for (auto iter = m_rules.cbegin(); iter != m_rules.cend(); ++iter)
    {
        if ((iter->m_funcode == funcode) && (iter->m_address == startaddr) && (iter->m_regnum == regnum))
        {
            cout << "same rule has been existed in rules!" << endl;
            return -1;
        }
    }

    Modbus_Rule rule;

    auto ptr = RegistersMap();
    if (!ptr) //not pointer to anything
    {
        cout << "RegistersMap not register!" << endl;
        return -1;
    }

    //cout << "funcode:" << (int)(funcode) << "startaddr:" << (int)(startaddr) << "regnum:" << (int)(regnum) << endl;
    rule.m_pdata = ptr->Retrun_RegisterPlaceHolder(funcode, startaddr, regnum);
    if (rule.m_pdata == nullptr)
    {
        cout << "add rules not find correspond registeraddress!" << endl;
        return -1;
    }

    rule.m_funcode = funcode;
    rule.m_address = startaddr;
    rule.m_regnum = regnum;
    rule.m_interval = rule.m_current = interval;
    m_rules.push_back(rule);
    //cout << "m_rules size:" << m_rules.size() << endl;
    return 0;
}

int Modbus_Master::Register_Rule(unsigned char funcode, unsigned short startaddr, unsigned short regnum, initializer_list<unsigned short> &init)
{
    for (auto iter = m_rules.cbegin(); iter != m_rules.cend(); ++iter)
    {
        if ((iter->m_funcode == funcode) && (iter->m_address == startaddr) && (iter->m_regnum == regnum))
        {
            cout << "same rule has been existed in rules!" << endl;
            return -1;
        }
    }
    Modbus_Rule rule;

    auto ptr = RegistersMap();
    if (!ptr) //not pointer to anything
    {
        cout << "RegistersMap not register!" << endl;
        return -1;
    }

    rule.m_pdata = ptr->Retrun_RegisterPlaceHolder(funcode, startaddr, regnum);
    if (rule.m_pdata == nullptr)
    {
        cout << "add rules not find correspond registeraddress!" << endl;
        return -1;
    }

    //set default
    for (int i = 0; i < regnum; ++i)
    {
        *(rule.m_pdata + i) = *(init.begin() + i);
    }

    rule.m_funcode = funcode;
    rule.m_address = startaddr;
    rule.m_regnum = regnum;
    m_rules.push_back(rule);
    return 0;
}

void Modbus_Master::Add_Rules(unsigned char funcode, unsigned short startaddr, unsigned short regnum, initializer_list<unsigned short> init)
{
    if (funcode != 0x06 && funcode != 0x10)
    {
        return;
    }

    if (regnum > init.size())
    {
        cout << "initlist less than regnum!" << endl;
        return;
    }
    Register_Rule(funcode, startaddr, regnum, init);
    return;
}

void Modbus_Master::Add_Rules(unsigned char funcode, unsigned short startaddr, unsigned short regnum, unsigned short interval)
{
    int group = regnum / 100; //need divide into serveral groups
    int left = regnum % 100;
    if (group != 0)
    {
        for (int i = 0; i < group; ++i)
        {
            Register_Rule(funcode, startaddr + 100 * i, 100, interval);
        }
    }
    //to register left regnum
    if (left != 0)
    {
        unsigned short left_startaddress = startaddr + regnum - left;
        Register_Rule(funcode, left_startaddress, left, interval);
    }
    return;
}

int Modbus_Master::NextRule(Modbus_Rule &rule) //only for 0x03 0x04 funcode not for 0x06 0x10
{
    auto interval = Modbus_Base::UpdateTime();
    if (interval < 0) //disable timer
    {
        //so only select one of rule
        static int disable_index = 0;
        //cout << "disable_index:" << disable_index << endl;
        for (; disable_index < m_rules.size();)
        {
            if (m_rules[disable_index].m_funcode == 0x03 || m_rules[disable_index].m_funcode == 0x04)
            {
                rule = m_rules[disable_index];
                disable_index++;
                if (disable_index == m_rules.size())
                {
                    disable_index = 0;
                }
                return 0;
            }
            else
            {
                disable_index++;
                if (disable_index == m_rules.size())
                {
                    disable_index = 0;
                }
            }
        }
    }
    else
    {
        //enable timer update interval

        for (auto iter = m_rules.begin(); iter != m_rules.end(); ++iter)
        {
            if (iter->m_funcode == 0x03 || iter->m_funcode == 0x04)
            {
                if (iter->m_current <= interval)
                {
                    iter->m_current = 0;
                }
                else
                {
                    iter->m_current -= interval;
                }
            }
        }

        static int enable_index = 0;
        for (; enable_index < m_rules.size();)
        {
            if (m_rules[enable_index].m_funcode == 0x03 || m_rules[enable_index].m_funcode == 0x04)
            {
                if (m_rules[enable_index].m_current == 0) //if time out
                {
                    rule = m_rules[enable_index];
                    m_rules[enable_index].m_current = m_rules[enable_index].m_interval;
                    enable_index++;
                    if (enable_index == m_rules.size())
                    {
                        enable_index = 0;
                    }
                    return 0;
                }
                else
                { //time not out
                    enable_index++;
                    if (enable_index == m_rules.size())
                    {
                        enable_index = 0;
                        return -1;
                    }
                }
            }
            else
            { //for 0x06 0x10
                enable_index++;
                if (enable_index == m_rules.size())
                {
                    enable_index = 0;
                    return -1;
                }
            }
        }
    }
    return 0;
}

int Modbus_Master::Rule0x03_04ToMsg(const Modbus_Rule &rule, std::vector<unsigned char> &data)
{
    data.push_back(Modbus_Base::Address());
    data.push_back(rule.m_funcode);
    data.push_back((rule.m_address >> 8) & 0xff);
    data.push_back(rule.m_address & 0xff);
    data.push_back((rule.m_regnum >> 8) & 0xff);
    data.push_back(rule.m_regnum & 0xff);
    return 0;
}

int Modbus_Master::Rule0x06ToMsg(const Modbus_Rule &rule, std::vector<unsigned char> &data)
{
    data.push_back(Modbus_Base::Address());
    data.push_back(rule.m_funcode);
    data.push_back((rule.m_address >> 8) & 0xff);
    data.push_back(rule.m_address & 0xff);

    //unsigned short setting = *(rule.m_pdata);
    data.push_back((rule.m_setting >> 8) & 0xff);
    data.push_back(rule.m_setting & 0xff);
    return 0;
}

int Modbus_Master::Rule0x10ToMsg(const Modbus_Rule &rule, std::vector<unsigned char> &data)
{
    data.push_back(Modbus_Base::Address());
    data.push_back(rule.m_funcode);
    data.push_back((rule.m_address >> 8) & 0xff);
    data.push_back(rule.m_address & 0xff);
    data.push_back((rule.m_regnum >> 8) & 0xff);
    data.push_back(rule.m_regnum & 0xff);

    unsigned short setting;
    if (rule.m_update_oneormore)
    {
        for(int i = 0; i < rule.m_regnum; i++)
        {
            if(i >= rule.m_settingoffset && i <= rule.m_settingoffset + rule.m_settings.size())
            {
                setting = rule.m_settings[i - rule.m_settingoffset];
            }else{
                setting = *(rule.m_pdata + i);
            }
            data.push_back((setting >> 8) & 0xff);
            data.push_back(setting & 0xff);
        }
    }
    else
    {

        for (int i = 0; i < rule.m_regnum; i++)
        {
            if (i == rule.m_settingoffset)
            {
                setting = rule.m_setting;
            }
            else
            {
                setting = *(rule.m_pdata + i);
            }
            data.push_back((setting >> 8) & 0xff);
            data.push_back(setting & 0xff);
        }
    }
    return 0;
}

int Modbus_Master::RuleToMsg(const Modbus_Rule &rule, std::vector<unsigned char> &data)
{
    if (rule.m_funcode == 0x03 || rule.m_funcode == 0x04)
    {
        Rule0x03_04ToMsg(rule, data);
    }
    else if (rule.m_funcode == 0x06)
    {
        Rule0x06ToMsg(rule, data);
    }
    else if (rule.m_funcode == 0x10)
    {
        Rule0x10ToMsg(rule, data);
    }

    shared_ptr<Modbus_Protocol> pro = Modbus_Base::Protocol();
    pro->Generate(data);
    return 0;
}

int Modbus_Master::Send(unsigned char *pbuf, unsigned short &len)
{
    //check if valid rule
    Modbus_Rule rule;
    if (NextRule(rule) >= 0)
    {
        //add to msgqueue
        //cout << "funcode:" << (int)rule.m_funcode << "startaddress:" << (int)rule.m_address << "regnum:" << (int)rule.m_regnum << endl;
        m_rulequeue.push(rule);
        if (m_rulequeue.size() >= 50)
        {
            cout << "rulequeue size is too larger!" << endl;
        }
    }

    cout << "rulequeue size:" << (int)m_rulequeue.size() << endl;
    if (m_rulequeue.empty())
    {
        cout << "send msgqueue is empty!" << endl;
        return -1;
    }

    Modbus_Rule sendrule = m_rulequeue.front(); //just get but not remove when recv the data pop the rule
    //m_rulequeue.pop();

    vector<unsigned char> data;
    RuleToMsg(sendrule, data); // transform rule to msg
    memcpy(pbuf, &data[0], data.size());
    len = data.size();
    return 0;
}

int Modbus_Master::UpdateData(const Modbus_Rule &rule, const vector<unsigned char> &data)
{
    unsigned char recv_address = data[0];
    if (recv_address != Modbus_Base::Address())
    {
        cout << "modbus address invalid!" << endl;
        return -1;
    }

    unsigned char recv_funcode = data[1];
    if (recv_funcode != rule.m_funcode)
    {
        cout << "funcode invalid!" << endl;
        return -1;
    }

    if (recv_funcode == 0x03 || recv_funcode == 0x04)
    {
        unsigned short recv_length = data[2];
        if (recv_length != (rule.m_regnum * 2))
        {
            cout << "length invalid!" << endl;
            return -1;
        }

        //update data
        //cout << "dataaddress:" << (int)rule.m_pdata << endl;
        for (int i = 0; i < rule.m_regnum; i++)
        {
            *(rule.m_pdata + i) = (unsigned short)((data[3 + i << 1] << 8) | (data[4 + i << 1]));
        }
    }
    else if (recv_funcode == 0x06)
    {
        unsigned short recv0x06_address = (unsigned short)((data[2] << 8) | data[3]);
        if (recv0x06_address != rule.m_address)
        {
            return -1;
        }

        unsigned short recv_data = (unsigned short)((data[4] << 8) | data[5]);
        if (recv_data != rule.m_setting)
        {
            return -1;
        }

        *(rule.m_pdata) = rule.m_setting;
    }
    else if (recv_funcode == 0x10)
    {
        unsigned short recv0x10_address = (unsigned short)((data[2] << 8) | data[3]);
        if (recv0x10_address != rule.m_address)
        {
            return -1;
        }
        unsigned short recv_regnum = (unsigned short)((data[4] << 8) | data[5]);
        if (recv_regnum != rule.m_regnum)
        {
            return -1;
        }

        *(rule.m_pdata + rule.m_settingoffset) = rule.m_setting;
    }

    return 0;
}

int Modbus_Master::Recv(unsigned char *pbuf, unsigned short len)
{
    vector<unsigned char> data;
    for (int i = 0; i < len; i++)
    {
        data.push_back(*(pbuf + i));
    }

    shared_ptr<Modbus_Protocol> pro = Modbus_Base::Protocol();
    if (pro->Parse(data) < 0)
    {
        cout << "recv data check failed!" << endl;
        return -1;
    }

    Modbus_Rule recvrule = m_rulequeue.front();
    if (UpdateData(recvrule, data) < 0)
    {
        cout << "recv data info not equal rule" << endl;
        return -1;
    }
    m_rulequeue.pop();
    return 0;
}

int Modbus_Master::UpdateData(unsigned short address, unsigned short data)
{
    /*
    if (Modbus_Base::UpdateData(address, data) < 0)
    {
        return -1;
    }
    */
    //prepare join in send msg
    for (auto iter = m_rules.begin(); iter != m_rules.end(); ++iter)
    {
        if (iter->m_funcode == 0x06 || iter->m_funcode == 0x10)
        {
            unsigned endaddress = iter->m_address + iter->m_regnum - 1;
            if (address >= iter->m_address && address <= endaddress)
            {
                iter->m_setting = data;
                iter->m_settingoffset = address - iter->m_address;
                if (iter->m_funcode == 0x10)
                {
                    iter->m_update_oneormore = false;
                }
                m_rulequeue.push(*iter);
                break;
            }
        }
    }
    return 0;
}

int Modbus_Master::UpdateData(unsigned short address, short data)
{
    UpdateData(address, (unsigned short)data);
    return 0;
}

int Modbus_Master::UpdateData(unsigned short address, unsigned short regnum, std::initializer_list<unsigned short> init)
{
    for (auto iter = m_rules.begin(); iter != m_rules.end(); ++iter)
    {
        if (iter->m_funcode == 0x10)
        {
            unsigned endaddress = iter->m_address + iter->m_regnum - 1;
            if (address >= iter->m_address && address <= endaddress)
            {
                if ((address + regnum - 1) <= endaddress)
                {
                    //save data
                    iter->m_settings = vector<unsigned short>(init.begin(), init.end());
                    iter->m_update_oneormore = true;
                    iter->m_settingoffset = address - iter->m_address;
                    m_rulequeue.push(*iter);
                }
                break;
            }
        }
    }
    return 0;
}