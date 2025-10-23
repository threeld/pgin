package tui

import (
	"fmt"

	"github.com/ThreeLD/pgin/client/internal"
	tea "github.com/charmbracelet/bubbletea"
)

type serverMsg struct{ text string }
type errMsg struct{ err error }

type model struct {
	message string
	err     error
}

func (m model) Init() tea.Cmd {
	return func() tea.Msg {
		msg, err := internal.Msg()
		if err != nil {
			return errMsg{err}
		}
		return serverMsg{msg}
	}
}

func (m model) Update(msg tea.Msg) (tea.Model, tea.Cmd) {
	switch msg := msg.(type) {
	case serverMsg:
		m.message = msg.text
	case errMsg:
		m.err = msg.err

	case tea.KeyMsg:
		switch msg.Type {
		case tea.KeyCtrlC, tea.KeyEsc:
			return m, tea.Quit
		}
	}
	return m, nil
}

func (m model) View() string {
	if m.err != nil {
		return fmt.Sprintf("Error: %v\n", m.err)
	} else {
		return fmt.Sprintf("Message: %s\n", m.message)
	}
}

func NewModel() model { return model{} }
